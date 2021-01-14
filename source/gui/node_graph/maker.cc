#include <maker.h>
#include <vinacomp.h>
#include <panels_layout.h>
#include <panel.h>

maker::maker(
	QWidget *__vinacomp,
    properties *__properties,
    nodes_load *_nodes_loaded,
    node_view *__node_view)

    : _vinacomp(__vinacomp)
	, _properties(__properties)
    , nodes_loaded(_nodes_loaded)
    , _node_view(__node_view)
{

    finder = new node_finder(_node_view, nodes_loaded);
    connect(finder->tree, &QTreeWidget::itemPressed, this, [this](QTreeWidgetItem *item) {
        QString node_id = item->text(1);
        create_fx(node_id);
        finder->hide();
    });

    setup_shortcut();
}

maker::~maker()
{
}

void maker::setup_shortcut()
{
    qt::shortcut("Tab", _node_view, [this]() {
        finder->show_finder();
    });

    qt::shortcut("Escape", _node_view, [this]() {
        finder->hide();
    });

    qt::shortcut("G", _node_view, [this]() {
        create_fx("grade");
    });

    qt::shortcut("T", _node_view, [this]() {
        create_fx("transform");
    });

    qt::shortcut("B", _node_view, [this]() {
        create_fx("blur");
    });

    qt::shortcut("M", _node_view, [this]() {
        create_fx("merge");
    });

    qt::shortcut("R", _node_view, [this]() {
        create_fx("read");
    });

    qt::shortcut("W", _node_view, [this]() {
        create_fx("write");
    });

    qt::shortcut("K", _node_view, [this]() {
        create_fx("copy");
    });
}

QString maker::create_fx(QString id)
{

    QJsonObject effect = nodes_loaded->get_effect(id);
    if (effect.empty())
        return 0;

    QString group = effect["group"].toString();
    QString label = effect["label"].toString();
    QString icon_name = effect["icon"].toString();
    QJsonArray knobs = effect["knobs"].toArray();
    QColor color = default_color(group);

    // Encuentra un nombre disponible
    QString name;
    int node_number = 1;
    while (true)
    {
        name = label + QString::number(node_number);
        if (!_node_view->get_node(name))
            break;
        node_number++;
    }
    //
    //

    // Crear panel de 'knobs'
	QStringList nodes_without_panel = {"viewer", "dot", "backdrop"};

	trim_panel *_trim_panel = nullptr;
	if (!nodes_without_panel.contains(id))
	{
		_trim_panel = new trim_panel(
			_properties,
			name,
			icon_name,
			&knobs);
		_properties->add_trim_panel(_trim_panel);
	}
    //
    //

	// Viewer
	viewer *_viewer = nullptr;
	if (id == "viewer")
	{
		_viewer = new viewer();
		panels_layout *_panels_layout = dynamic_cast<vinacomp *>(_vinacomp)->get_panels_layout();
		panel *viewer_panel = _panels_layout->get_viewer_panel();
		if (viewer_panel)
			viewer_panel->add_viewer(_viewer, name);
	}
	//

    // Creación del nodo, con un número que no se ha utilizado.
    _node_view->create_node(name, _trim_panel, _viewer, icon_name, color);
    //
    //

    return name;
}

QColor maker::default_color(QString effect_group)
{
    QMap<QString, QColor> colors;

    colors.insert("draw", QColor(76, 128, 51));
    colors.insert("time", QColor(175, 163, 93));
    colors.insert("channel", QColor(158, 59, 98));
    colors.insert("color", QColor(122, 168, 255));
    colors.insert("filter", QColor(204, 128, 77));
    colors.insert("keyer", QColor(0, 200, 0));
    colors.insert("merge", QColor(75, 93, 198));
    colors.insert("transform", QColor(165, 121, 170));

    if (colors.contains(effect_group))
        return colors.value(effect_group);
    else
        return QColor(150, 150, 150);
}
