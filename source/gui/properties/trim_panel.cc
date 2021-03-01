#include <trim_panel.h>
#include <vinacomp.h>

trim_panel::trim_panel(properties *__properties,
                       QString _name,
					   QString _type,
                       QString _icon_name,
					   nodes_load *_nodes_loaded,
					   project_struct *project,
					   QWidget *__vinacomp
					   )

    : _properties(__properties)
	, name(_name)
	, type(_type)
	, icon_name(_icon_name)
	, nodes_loaded(_nodes_loaded)
	, data(project->nodes[_name].params)
	, _vinacomp(__vinacomp)

	, knob_editor_visible(false)
	, _knob_editor(nullptr)
	, _node_gui(nullptr)
{
	knobs = new QMap<QString, knob*>;

	this->setObjectName("trim_panel");
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	setup_ui();

	// obtiene la lista de viewers en una lista de viewers pero con 'QWidget'
	// para usarlos con dynamic_cast y no tener que importar el viewer.h a cada knob
	QList <QWidget*> *viewers_gl = dynamic_cast<vinacomp*>(_vinacomp)->get_viewers_gl();
	//

	QJsonArray _knobs = nodes_loaded->get_effect(type).value("knobs").toArray();
	setup_knobs(_knobs, controls_layout, viewers_gl);

	QJsonArray shared_knobs = jread("source/engine/nodes/json/shared_params.json").value("knobs").toArray();
	setup_knobs(shared_knobs, node_tab_layout, viewers_gl);

	setup_gui_panels(_knobs);
}

trim_panel::~trim_panel()
{
}

void trim_panel::setup_ui()
{
    layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	layout->setSpacing(0);
    layout->setMargin(0);

    QWidget *butttons = top_buttons_setup_ui();
    layout->addWidget(butttons);

	knob_editor_container = new QWidget(this);
	layout->addWidget(knob_editor_container);

    tabs = tabs_ui();
    layout->addWidget(tabs);
}

void trim_panel::setup_gui_panels(QJsonArray _knobs)
{
	// todos estos nodo gui son solo si el nodo efecto tiene algun boton
	// u otra interface adicional a las que se generan en 'setup_knobs'
	// por eso solo son algunos nodos y no todos
	if (type == "frame_range")
		_node_gui = new frame_range_gui();
	else if (type == "reformat")
		_node_gui = new reformat_gui();
	else if (type == "shuffle")
	{
		QString name = "shuffle";
		QJsonObject default_value = _knobs[0].toObject().value("default").toObject();
		QJsonObject value;

		if (data->contains(name))
			value = data->value(name).toObject();
		else
			value = default_value;

		shuffle_gui *shuffle = new shuffle_gui(controls_layout, value);

		connect(shuffle, &shuffle_gui::changed, this, [=](QJsonObject _data){
			if (default_value != _data)
				data->insert(name, _data);
			else
				data->remove(name);
			update_render();
		});
		_node_gui = shuffle;
	}

	if (_node_gui)
		_node_gui->setup(this, _vinacomp, name);
}

QWidget *trim_panel::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("butttons"); widget->setMaximumHeight(30);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setMargin(5);
    //
    //

    int icon_size = 20;

    QPushButton *center_node = new QPushButton(widget);
    qt::set_icon(center_node, "center_a", icon_size);
    layout->addWidget(center_node);

    knob_editor_button = new QPushButton(widget);
    connect(knob_editor_button, &QPushButton::clicked, this, [=]() {
		knob_editor_toggle();
    });
    qt::set_icon(knob_editor_button, "edit_a", icon_size);
    layout->addWidget(knob_editor_button);

    layout->addStretch();

    QPushButton *icon_node = new QPushButton(widget);
    qt::set_icon(icon_node, icon_name + "_a", icon_size);
    layout->addWidget(icon_node);

    name_edit = new QLineEdit(widget);
    name_edit->setText(name);
    layout->addWidget(name_edit);

    layout->addStretch();

    // Minimize
    QPushButton *minimize = new QPushButton(widget);
    connect(minimize, &QPushButton::clicked, this, [=]() {
        this->maximize(!is_maximize);
    });
    qt::set_icon(minimize, "minimize_a", icon_size);
    layout->addWidget(minimize);
    // Minimize
    //

    QPushButton *maximize_button = new QPushButton(widget);
    qt::set_icon(maximize_button, "maximize_a", icon_size);
    layout->addWidget(maximize_button);

    QPushButton *close = new QPushButton(widget);
    connect(close, &QPushButton::clicked, this, [this]() {
        _properties->close_trim_panel(this->get_name());
    });
    qt::set_icon(close, "close_a", icon_size);
    layout->addWidget(close);

    return widget;
}

tab_widget *trim_panel::tabs_ui()
{
    tab_widget *tabs = new tab_widget();
    tabs->setParent(this);

    controls_tab = new QWidget(this);
    controls_layout = new QVBoxLayout(controls_tab);
    controls_layout->setSpacing(5);
    controls_tab->setObjectName("controls");

    QWidget *node_tab = new QWidget(this);
    node_tab->setObjectName("node_tab");
	node_tab_layout = new QVBoxLayout(node_tab);
    node_tab_layout->setSpacing(5);

	tabs->add_tab(controls_tab, "Controls");
	tabs->add_tab(node_tab, "Node");

    tabs->set_index(0);

    return tabs;
}

void trim_panel::knob_editor_toggle()
{
	if (!_knob_editor)
	{
		// el 'knob_editor' solo se crea cuando se presiona el boton se 'edit', ya que
		// solo se usa para editar los knobs, y solo se crea si no esta creado antes
		QHBoxLayout *knob_editor_layout = new QHBoxLayout(knob_editor_container);
		knob_editor_layout->setMargin(0);
		_knob_editor = new knob_editor();
		_knob_editor->hide();
		knob_editor_layout->addWidget(_knob_editor);
	}

	knob_editor_visible = !knob_editor_visible;
	_knob_editor->setVisible(knob_editor_visible);

	if (knob_editor_visible)
		qt::set_icon(knob_editor_button, "edit_c", 20);
	else
		qt::set_icon(knob_editor_button, "edit_a", 20);
}

knob *trim_panel::get_knob(QString name) const
{
	return knobs->value(name);
}

QString trim_panel::get_name() const
{
    return name;
}

QString trim_panel::get_type() const
{
	return type;
}

void trim_panel::set_name(QString _name)
{
    name = _name;
    name_edit->setText(name);
}

void trim_panel::maximize(bool _maximize)
{
    tabs->setVisible(_maximize);
    is_maximize = _maximize;
}

void trim_panel::update_render()
{
	dynamic_cast<vinacomp*>(_vinacomp)->update_render_all_viewer();
}
