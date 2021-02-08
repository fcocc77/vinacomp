#include <trim_panel.h>

void trim_panel::setup_knobs(QJsonArray _knobs, QVBoxLayout *layout)
{
    // Obtiene el ancho maximo a partir de las 'label'
    // para usarlo en el espacio inicial de cada parametro.
    int init_space_width = 0;
    for (int i = 0; i < _knobs.count(); i++)
    {
        QJsonObject knob_object = _knobs.at(i).toObject();
        QString type = knob_object.value("type").toString();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

        if (!over_line)
        {
            QLabel _label(label);
            int width = _label.fontMetrics().boundingRect(_label.text()).width();

			if (type == "button" || type == "label")
				width = 0;

            if (width > init_space_width)
                init_space_width = width;
        }
    }
    //
    //

    QList<knob *> over_line_knobs;
    for (int i = 0; i < _knobs.count(); i++)
    {
        QJsonObject knob_object = _knobs.at(i).toObject();
        QString type = knob_object.value("type").toString();
        QString name = knob_object.value("name").toString();
        QString label = knob_object.value("label").toString();
        bool over_line = knob_object.value("over_line").toBool();

		bool animatable = true;
		if (knob_object.contains("animatable"))
			animatable = knob_object.value("animatable").toBool();

		bool visible = true;
		if (knob_object.contains("visible"))
			visible = knob_object.value("visible").toBool();

        knob *_knob = nullptr;
        if (type == "color")
        {
            QJsonArray _default = knob_object.value("default").toArray();

			float dred = _default.at(0).toDouble();
			float dgreen = _default.at(1).toDouble();
			float dblue = _default.at(2).toDouble();
			float dalpha = _default.at(3).toDouble();

			float red, green, blue, alpha;

			if (data->contains(name))
			{
				QJsonArray value = data->value(name).toArray();
				red = value.at(0).toDouble();
				green = value.at(1).toDouble();
				blue = value.at(2).toDouble();
				alpha = value.at(3).toDouble();
			}
			else
			{
				red = dred;
				green = dgreen;
				blue = dblue;
				alpha = dalpha;
			}


            knob_color *_knob_color = new knob_color(
				knob_object.value("minimum").toDouble(),
				knob_object.value("maximum").toDouble(),
				red, green, blue, alpha
			);

			connect(_knob_color, &knob_color::changed, this, [=](float r, float g, float b, float a){
				// si el color es igual al predeterminado, borra el dato
				if (dred != r || dgreen != g || dblue != b || dalpha != a)
					data->insert(name, QJsonArray{r, g, b, a});
				else
					data->remove(name);

				update_render();
			});

			_knob = _knob_color;
        }

        else if (type == "check_box")
        {
            bool default_value = knob_object.value("default").toBool();
			bool value;

			if (data->contains(name))
				value = data->value(name).toBool();
			else
				value = default_value;

            knob_check_box *_knob_check_box = new knob_check_box(label, value);

			connect(_knob_check_box, &knob_check_box::changed, this, [=](bool _value){
				if (default_value != _value)
					data->insert(name, _value);
				else
					data->remove(name);

				update_render();
			});

            label = "";
			_knob = _knob_check_box;
        }

        else if (type == "file")
        {
			QString default_value = knob_object.value("default").toString();
			QString value;

			if (data->contains(name))
				value = data->value(name).toString();
			else
				value = default_value;

            knob_file *_knob_file = new knob_file(value);

			connect(_knob_file, &knob_file::changed, this, [=](QString file_path){
				if (default_value != file_path)
					data->insert(name, file_path);
				else
					data->remove(name);
				update_render();
			});

			_knob = _knob_file;
        }

        else if (type == "choice")
        {
            QJsonArray items = knob_object.value("items").toArray();
			QJsonArray default_value = knob_object.value("default").toArray();
			int default_index = default_value[0].toInt();
			int index;

			if (data->contains(name))
				index = data->value(name).toInt();
			else
				index = default_index;

			// convierte la lista de items
			QList <pair<QString, QVariant>> _items;
			for (QJsonValue item : items)
			{
				QJsonArray _item = item.toArray();
				QString label = _item[0].toString();
				QJsonValue value = _item[1];
				_items.push_back({label, value});
			}
			//

            knob_choice *_knob_choice = new knob_choice(_items, index);

			connect(_knob_choice, &knob_choice::changed, this, [=](QVariant value, int _index){
				if (default_index != _index)
					data->insert(name, QJsonArray{_index, value.toJsonValue()});
				else
					data->remove(name);

				if (_node_gui)
				   _node_gui->changed(name);
				update_render();
			});

			_knob = _knob_choice;
        }

        else if (type == "text")
        {
			QString default_text = knob_object.value("default").toString();
			QString text;

			if (data->contains(name))
				text = data->value(name).toString();
			else
				text = default_text;

            knob_text *_knob_text = new knob_text(text);

			connect(_knob_text, &knob_text::changed, this, [=](QString _text){
				if (default_text != _text)
					data->insert(name, _text);
				else
					data->remove(name);
				update_render();
			});

			_knob = _knob_text;
        }

        else if (type == "label")
        {
            _knob = new knob_label(label);
        }

        else if (type == "button")
        {
            knob_button *_knob_button = new knob_button(label);
			label = "";

			connect(_knob_button, &knob_button::clicked, this, [=](){
				if (_node_gui)
				   _node_gui->changed(name);
			});

			_knob = _knob_button;
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
			int default_value = knob_object.value("default").toInt();
			int value;

			if (data->contains(name))
				value = data->value(name).toInt();
			else
				value = default_value;

			knob_integer *_knob_integer = new knob_integer(
				knob_object.value("minimum").toInt(),
				knob_object.value("maximum").toInt(),
				value
			);

			connect(_knob_integer, &knob_integer::changed, this, [=](int _value){
				if (default_value != _value)
					data->insert(name, _value);
				else
					data->remove(name);
				update_render();
			});

			_knob = _knob_integer;
        }

        else if (type == "floating")
        {
            float default_value = knob_object.value("default").toDouble();
			float value;

			if (data->contains(name))
				value = data->value(name).toDouble();
			else
				value = default_value;

			knob_floating *_knob_floating = new knob_floating(
				knob_object.value("minimum").toDouble(),
				knob_object.value("maximum").toDouble(),
				value
			);

			connect(_knob_floating, &knob_floating::changed, this, [=](float _value){
				if (default_value != _value)
					data->insert(name, _value);
				else
					data->remove(name);
				update_render();
			});

			_knob = _knob_floating;
        }

        else if (type == "separator")
        {
            _knob = new knob_separator();
        }

        else if (type == "floating_dimensions")
		{
			QList <float> default_dimensions = {0, 0};
			knob_floatd *knob_floating_dimensions = new knob_floatd(default_dimensions);

			_knob = knob_floating_dimensions;
		}

        else if (type == "integer_dimensions")
        {
			QList <int> default_dimensions, dimensions;
			for (QJsonValue value : knob_object.value("default").toArray())
				default_dimensions.push_back(value.toInt());

			if (data->contains(name))
				for (QJsonValue value : data->value(name).toArray())
					dimensions.push_back(value.toInt());
			else
				dimensions = default_dimensions;

			knob_intd *knob_integer_dimensions = new knob_intd(dimensions);

			connect(knob_integer_dimensions, &knob_intd::changed, this, [=](QList <int> _dimensions){
				QJsonArray __dimensions;
				for (int value : _dimensions)
					__dimensions.push_back(value);

				if (_dimensions != default_dimensions)
					data->insert(name, __dimensions);
				else
					data->remove(name);
				update_render();
			});

			_knob = knob_integer_dimensions;
        }

        if (_knob)
        {
            if (over_line)
            {
                // si el parametro tiene 'over_line', crea un widget de linea si no esta creado
                // e inserta todos los knob anteriores que tengan 'over_line'
				QObject *parent = over_line_knobs.value(0)->parent();
				QWidget *line_widget;
				QHBoxLayout *line_layout;

				if (parent->objectName() == "line_widget")
				{
					line_widget = dynamic_cast<QWidget*>(parent);
					line_layout = dynamic_cast<QHBoxLayout*>(line_widget->layout());
				}
				else
				{
					line_widget = new QWidget(controls_tab);
					line_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
					line_widget->setObjectName("line_widget");

					line_layout = new QHBoxLayout(line_widget);
					line_layout->setMargin(0);
				}
				//
				//

				for (knob *last_knob : over_line_knobs)
				{
					line_layout->addWidget(last_knob);
					last_knob->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
				}

				line_layout->addWidget(_knob);

				_knob->set_init_space(0);
				layout->addWidget(line_widget);
            }
            else
            {
                over_line_knobs.clear();
				if (type == "label" || type == "separator")
					_knob->set_init_space(0);
				else
					_knob->set_init_space(init_space_width, label);

                layout->addWidget(_knob);
            }
			_knob->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

			_knob->set_animatable(animatable);
			_knob->set_visible(visible);
        }

		knobs->insert(name, _knob);
        over_line_knobs.push_back(_knob);
    }

	// itera todos lo 'knobs' y si el 'knob' es un grupo lo actualiza, para que oculte o muestre
	// los knob incluidos, si el grupo esta abierto o cerrado, no se puede hacer antes ya
	// que tienen que estar todos los 'knobs' creados.
	for (QString key : knobs->keys())
	{
		knob *_knob = knobs->value(key);
		knob_group *group = dynamic_cast<knob_group *>(_knob);
		if (group)
			group->update();
	}
	//
}
