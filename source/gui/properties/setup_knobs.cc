#include <knob_button.h>
#include <knob_channels.h>
#include <knob_check_box.h>
#include <knob_choice.h>
#include <knob_color.h>
#include <knob_file.h>
#include <knob_floatd.h>
#include <knob_floating.h>
#include <knob_group.h>
#include <knob_intd.h>
#include <knob_integer.h>
#include <knob_label.h>
#include <knob_progress.h>
#include <knob_separator.h>
#include <knob_text.h>
#include <setup_knobs.h>

#include <animation.h>
#include <curve_editor.h>
#include <trim_panel.h>

void setup_knobs(setup_knobs_props props)
{
    // Obtiene el ancho maximo a partir de las 'label'
    // para usarlo en el espacio inicial de cada parametro.
    int init_space_width = 0;
    for (int i = 0; i < props.knobs_array.count(); i++)
    {
        QJsonObject knob_object = props.knobs_array.at(i).toObject();
        QString type = knob_object.value("type").toString();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

        if (!over_line)
        {
            QLabel _label(label);
            int width =
                _label.fontMetrics().boundingRect(_label.text()).width();

            if (type == "button" || type == "label" || type == "check_box")
                width = 0;

            if (width > init_space_width)
                init_space_width = width;
        }
    }
    //
    //

    QList<knob *> over_line_knobs;
    for (int i = 0; i < props.knobs_array.count(); i++)
    {
        QJsonObject knob_object = props.knobs_array.at(i).toObject();
        QString type = knob_object.value("type").toString();
        float min = knob_object.value("min").toDouble();
        float max = knob_object.value("max").toDouble();
        float default_value = knob_object.value("default").toDouble();
        bool centered_handler = knob_object.value("centered_handler").toBool();
        int dimensions_count = knob_object.value("dimensions").toDouble();
        bool bidimensional = knob_object.value("bidimensional").toBool();
        QString name = knob_object.value("name").toString();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

        knob_props _knob_props = {props.panel,     props.project,
                                  props.vinacomp,  props.viewers_gl,
                                  props.this_node, props.knob_editor,
                                  knob_object,     props.params};

        knob *_knob = nullptr;
        if (type == "color")
            _knob = new knob_color(_knob_props, min, max, 0, 0, 0, 0,
                                   centered_handler);

        else if (type == "check_box")
            _knob = new knob_check_box(_knob_props);

        else if (type == "file")
            _knob = new knob_file(_knob_props);

        else if (type == "choice")
            _knob = new knob_choice(_knob_props);

        else if (type == "text")
            _knob = new knob_text(_knob_props);

        else if (type == "label")
            _knob = new knob_label(_knob_props);

        else if (type == "button")
            _knob = new knob_button(_knob_props);

        else if (type == "group")
            _knob = new knob_group(_knob_props);

        else if (type == "integer")
            _knob = new knob_integer(_knob_props, min, max, default_value,
                                     bidimensional, centered_handler);

        else if (type == "floating")
            _knob = new knob_floating(_knob_props, min, max, default_value,
                                      bidimensional, centered_handler);

        else if (type == "separator")
            _knob = new knob_separator(_knob_props);

        else if (type == "floating_dimensions")
            _knob = new knob_floatd(_knob_props, dimensions_count);

        else if (type == "integer_dimensions")
            _knob = new knob_intd(_knob_props, dimensions_count);

        else if (type == "channels")
            _knob = new knob_channels(_knob_props);

        else if (type == "progress")
            _knob = new knob_progress(_knob_props);

        if (_knob)
        {
            if (over_line && !over_line_knobs.empty())
            {
                // si el parametro tiene 'over_line', crea un widget de linea si
                // no esta creado e inserta todos los knob anteriores que tengan
                // 'over_line'
                QObject *parent = over_line_knobs.value(0)->parent();
                QWidget *line_widget;
                QHBoxLayout *line_layout;

                if (parent->objectName() == "line_widget")
                {
                    line_widget =
                        static_cast<QWidget *>(parent); // QObject -> QWidget
                    line_layout =
                        static_cast<QHBoxLayout *>(line_widget->layout());
                }
                else
                {
                    line_widget = new QWidget(props.layout->widget());
                    line_widget->setSizePolicy(QSizePolicy::Expanding,
                                               QSizePolicy::Fixed);
                    line_widget->setFixedHeight(25);
                    line_widget->setObjectName("line_widget");

                    line_layout = new QHBoxLayout(line_widget);
                    line_layout->setAlignment(Qt::AlignLeft);
                    line_layout->setMargin(0);
                }
                //
                //

                for (knob *last_knob : over_line_knobs)
                {
                    line_layout->addWidget(last_knob);
                    last_knob->set_over_line_widget(line_widget);
                    last_knob->setSizePolicy(QSizePolicy::Fixed,
                                             QSizePolicy::Fixed);
                }

                line_layout->addWidget(_knob);

                _knob->set_init_space(0);

                _knob->set_over_line_widget(line_widget);
                props.layout->addWidget(line_widget);
            }
            else
            {
                over_line_knobs.clear();
                if (type == "label" || type == "separator")
                    _knob->set_init_space(0);
                else if (type == "button" || type == "check_box")
                    _knob->set_init_space(init_space_width);
                else
                {
                    _knob->set_init_space(init_space_width);
                    _knob->set_init_label(true);
                }

                props.layout->addWidget(_knob);
            }

            _knob->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            _knob->set_data();
            _knob->restore_param();

            if (props._node_panel)
                QObject::connect(_knob, &knob::to_node_panel, props._node_panel,
                                 &node_panel::changed);
        }

        props.knobs->insert(name, _knob);
        over_line_knobs.push_back(_knob);
    }

    // itera todos lo 'knobs' y si el 'knob' es un grupo lo actualiza, para que
    // oculte o muestre los knob incluidos, si el grupo esta abierto o cerrado,
    // no se puede hacer antes ya que tienen que estar todos los 'knobs'
    // creados.
    for (QString key : props.knobs->keys())
    {
        knob *_knob = props.knobs->value(key);
        knob_group *group = dynamic_cast<knob_group *>(_knob);
        if (group)
            group->update();
    }
    //
}
