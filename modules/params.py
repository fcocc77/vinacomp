import __py_params__ # type: ignore

class params:
    def __init__(self, node_name, param_name):
        self.node_name = node_name
        self.param_name = param_name

    def get_name(self) -> str:
        return self.param_name

    def get_value(self):
        return __py_params__.get_value(self.node_name, self.param_name);

    def get_color(self) -> list:
        return __py_params__.get_color(self.node_name, self.param_name);

    def get_index(self) -> int:
        return __py_params__.get_index(self.node_name, self.param_name);

    def set_value(self, value: float) -> float:
        return __py_params__.set_value(self.node_name, self.param_name, value);


def get_param(node_name: str, param_name: str):
    if __py_params__.param_exists(node_name, param_name):
        return params(node_name, param_name)
    else:
        print('Error: The \'' + param_name + '\' param does not exist.')
        return None

