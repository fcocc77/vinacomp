#include <trim_panel.h>

trim_panel::trim_panel(properties *__properties,
                       QString _name,
                       QString _icon_name,
                       QJsonArray *_knobs)

    : _properties(__properties)
	, name(_name)
	, icon_name(_icon_name)
	, knob_editor_visible(false)
	, _knob_editor(nullptr)
{

	this->setObjectName("trim_panel");
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	data = new QJsonObject({});

	setup_ui();
	setup_knobs(_knobs);
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

void trim_panel::setup_knobs(QJsonArray *knobs)
{
    // Obtiene el ancho maximo a partir de las 'label'
    // para usarlo en el espacio inicial de cada parametro.
    int init_space_width = 0;
    for (int i = 0; i < knobs->count(); i++)
    {
        QJsonObject knob_object = knobs->at(i).toObject();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

        if (!over_line)
        {
            QLabel _label(label);
            int width = _label.fontMetrics().boundingRect(_label.text()).width();

            if (width > init_space_width)
                init_space_width = width;
        }
    }
    //
    //

    QList<knob *> knob_list;
    QList<knob *> over_line_knobs;
    for (int i = 0; i < knobs->count(); i++)
    {
        QJsonObject knob_object = knobs->at(i).toObject();
        QString type = knob_object.value("type").toString();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

        knob *_knob;
        if (type == "color")
        {
            QJsonArray _default = knob_object.value("default").toArray();
            _knob = new knob_color(
                QColor(
                    _default.at(0).toInt(),
                    _default.at(1).toInt(),
                    _default.at(2).toInt()));
        }

        else if (type == "check_box")
        {
            bool _default = knob_object.value("default").toBool();
            _knob = new knob_check_box(
                label,
                _default);

            label = "";
        }

        else if (type == "file")
        {
            _knob = new knob_file();
        }

        else if (type == "choice")
        {
            QJsonArray items = knob_object.value("items").toArray();
            _knob = new knob_choice(qt::array_to_list(items));
        }

        else if (type == "text")
        {
            _knob = new knob_text();
        }

        else if (type == "label")
        {
            _knob = new knob_label(label);
        }

        else if (type == "button")
        {
            _knob = new knob_button();
        }

        else if (type == "group")
        {
            int knobs_included = knob_object.value("knobs").toInt();
            bool open_group = knob_object.value("open").toBool();
            _knob = new knob_group(label, knobs_included, open_group);
            label = "";
        }

        else if (type == "integer")
        {
            _knob = new knob_integer();
        }

        else if (type == "floating")
        {
            float _default = knob_object.value("default").toDouble();
            _knob = new knob_floating(_default);
        }

        else if (type == "separator")
        {
            _knob = new knob_separator();
        }

        else if (type == "position")
        {
            _knob = new knob_dimensions();
        }

        if (_knob)
        {
			_knob->set_animatable();

            if (over_line)
            {
                // si el parametro tiene 'over_line', crea un widget de linea
                // e inserta todos los knob anteriores que tengan 'over_line'
                QWidget *line_widget = new QWidget(controls_tab);
                QHBoxLayout *line_layout = new QHBoxLayout(line_widget);

                line_layout->setMargin(0);

                for (knob *last_knob : over_line_knobs)
                {
                    line_layout->addWidget(last_knob);
					last_knob->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                }

                line_layout->addWidget(_knob);
                line_layout->addStretch();

                _knob->set_init_space(0);
                controls_layout->addWidget(line_widget);
            }
            else
            {
                over_line_knobs.clear();
				if (type == "label" || type == "separator")
					_knob->set_init_space(0);
				else
					_knob->set_init_space(init_space_width, label);

                controls_layout->addWidget(_knob);
            }
			_knob->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        }
        knob_list.push_back(_knob);
        over_line_knobs.push_back(_knob);
    }

	// itera todos lo 'knobs' y si el 'knob' es un grupo lo actualiza, para que oculte o muestre
	// los knob incluidos, si el grupo esta abierto o cerrado, no se puede hacer antes ya
	// que tienen que estar todos los 'knobs' creados.
	for (knob *_knob : knob_list)
	{
		knob_group *group = dynamic_cast<knob_group *>(_knob);
		if (group)
			group->update();
	}
	//
}

QWidget *trim_panel::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("butttons");
    widget->setMaximumHeight(30);

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
    qt::set_icon(icon_node, icon_name, icon_size);
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

    QWidget *node = new QWidget(this);

	tabs->add_tab(controls_tab, "Controls");
	tabs->add_tab(node, "Node");

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

QString trim_panel::get_name()
{
    return name;
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

QJsonObject *trim_panel::get_data() const
{
	return data;
}

QJsonObject *trim_panel::get_modified_data() const
{
	// solo obtiene los datos de los parametros modificados
	return data;
}
