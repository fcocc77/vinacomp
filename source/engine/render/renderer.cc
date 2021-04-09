#include <renderer.h>

#include <bbox_adjust_node.h>
#include <blur_node.h>
#include <checker_board_node.h>
#include <clamp_node.h>
#include <constant_node.h>
#include <copy_node.h>
#include <cornel_pin_node.h>
#include <crop_node.h>
#include <dissolve_node.h>
#include <edge_detect_node.h>
#include <erode_node.h>
#include <frame_hold_node.h>
#include <grade_node.h>
#include <grid_node.h>
#include <idistort_node.h>
#include <invert_node.h>
#include <keyer_node.h>
#include <lens_distortion_node.h>
#include <merge_node.h>
#include <mirror_node.h>
#include <noise_node.h>
#include <position_node.h>
#include <premult_node.h>
#include <radial_node.h>
#include <ramp_node.h>
#include <rectangle_node.h>
#include <reformat_node.h>
#include <render_data.h>
#include <saturation_node.h>
#include <sharpen_node.h>
#include <shuffle_node.h>
#include <switch_node.h>
#include <text_node.h>
#include <transform_node.h>
#include <unpremult_node.h>
#include <viewer_node.h>

renderer_thread::renderer_thread(project_struct *_project)
    : project(_project)
{
    // los nodos que se definen globalmente aparte de insertarlos a la lista
    // 'nodes' es porque tienen funciones aparte de 'render'
    time_offset = new time_offset_node();
    read = new read_node();
    _frame_range = new frame_range_node();

    nodes.insert("time_offset", time_offset);
    nodes.insert("read", read);
    nodes.insert("frame_range", _frame_range);
    //

    nodes.insert("viewer", new viewer_node());
    nodes.insert("blur", new blur_node());
    nodes.insert("position", new position_node());
    nodes.insert("invert", new invert_node());
    nodes.insert("reformat", new reformat_node());
    nodes.insert("constant", new constant_node());
    nodes.insert("ramp", new ramp_node());
    nodes.insert("radial", new radial_node());
    nodes.insert("noise", new noise_node());
    nodes.insert("shuffle", new shuffle_node());
    nodes.insert("grid", new grid_node());
    nodes.insert("text", new text_node());
    nodes.insert("checker_board", new checker_board_node());
    nodes.insert("mirror", new mirror_node());
    nodes.insert("sharpen", new sharpen_node());
    nodes.insert("cornel_pin", new cornel_pin_node());
    nodes.insert("rectangle", new rectangle_node());
    nodes.insert("frame_hold", new frame_hold_node());
    nodes.insert("copy", new copy_node());
    nodes.insert("clamp", new clamp_node());
    nodes.insert("saturation", new saturation_node());
    nodes.insert("grade", new grade_node());
    nodes.insert("edge_detect", new edge_detect_node());
    nodes.insert("erode", new erode_node());
    nodes.insert("keyer", new keyer_node());
    nodes.insert("dissolve", new dissolve_node());
    nodes.insert("merge", new merge_node());
    nodes.insert("premult", new premult_node());
    nodes.insert("unpremult", new unpremult_node());
    nodes.insert("switch", new switch_node());
    nodes.insert("bbox_adjust", new bbox_adjust_node());
    nodes.insert("crop", new crop_node());
    nodes.insert("idistort", new idistort_node());
    nodes.insert("lens_distortion", new lens_distortion_node());
    nodes.insert("transform", new transform_node());
}

renderer_thread::~renderer_thread() {}

QString renderer_thread::get_input_node(QString node_name, int input) const
{
    node_struct *node = &project->nodes[node_name];
    QString input_node = node->inputs.value("in0").toString();

    return input_node;
}

pair<int, int> renderer_thread::get_frame_range(QString node_name) const
{
    // calcula el 'frame range' de un nodo, sin renderizar el nodo,
    // el 'frame range' se encuentra solo en algunos nodos como
    // read, frame_range, retime, y todo los nodos que tengan 'frame_range'
    node_struct *node = &project->nodes[node_name];

    if (node->type == "read")
        return read->get_frame_range(node->params);
    else if (node->type == "frame_range")
        return _frame_range->get_frame_range(node->params);

    QString input_node = node->inputs.value("in0").toString();
    if (!input_node.isEmpty())
        return get_frame_range(input_node);

    return {};
}

void renderer_thread::recursive_render(render_data *rdata)
{
    if (!project->nodes.contains(rdata->root_node))
        return;

    node_struct *node = &project->nodes[rdata->root_node];
    node_engine *_node_engine = nodes.value(node->type);

    bool disable = false;
    if (_node_engine)
        disable = _node_engine->get(node->params, "disable_node").toBool();

    // los nodos de tiempo tienen que modificar todos los nodos entrantes
    // por eso estos nodos tienen que ir antes de renderizar las entradas
    if (node->type == "time_offset" && !disable)
        time_offset->set_offset(node->params, rdata->frame, rdata->root_node,
                                this);
    //

    // renderiza las entradas del nodo antes que el nodo
    QString input_node = node->inputs.value("in0").toString();
    if (!input_node.isEmpty())
    {
        rdata->root_node = input_node;
        recursive_render(rdata);
    }
    //

    // renderiza el nodo actual
    if (_node_engine && !disable)
        _node_engine->render(rdata, node->params);
    //
}

void renderer_thread::run_render(render_data *rdata, int render_id)
{
    recursive_render(rdata);
    finished_render(render_id);
}

renderer::renderer(project_struct *project)
    : rendering(false)
    , render_id(0)
    , rdata_thread(new render_data)
{

    _renderer_thread = new renderer_thread(project);
    _renderer_thread->moveToThread(&thread);
    thread.start();

    connect(_renderer_thread, &renderer_thread::finished_render, this,
            &renderer::thread_finished_render);

    connect(this, &renderer::thread_run_render, _renderer_thread,
            &renderer_thread::run_render);
}

void renderer::render(render_data _rdata)
{
    // para asegurarnos que el ultimo render que llego
    // se renderice, crea un id de render y guarda los datos
    // de render, y si el id del 'call back' del render no es
    // igual, vuelve a renderizar hasta que el id sea el mismo
    render_id++;
    last_rdata = _rdata;
    //

    if (rendering)
        return;

    rendering = true;

    *rdata_thread = last_rdata;
    thread_run_render(rdata_thread, render_id);
}

void renderer::thread_finished_render(int _render_id)
{
    rendering = false;
    finished_render(*rdata_thread);

    if (render_id != _render_id)
        render(last_rdata);
}
