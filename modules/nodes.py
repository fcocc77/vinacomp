import __py_nodes__


class node:

    def __init__(self, node_name):
        self.name = node_name

    def delete(self):
        return __py_nodes__.delete_node(self.name)

    def set_position(self, x, y):
        return __py_nodes__.set_position(self.name, x, y)

    def get_param(self, param_name):
        return ""

    def rename(self, name):
        __py_nodes__.rename(self.name, name)
        self.name = name


def create_node(node_name):
    name = __py_nodes__.create_node(node_name)
    return get_node(name)


def get_node(node_name):
    if __py_nodes__.node_exists(node_name):
        return node(node_name)
    else:
        print('Error: The \'' + node_name + '\' node does not exist.')
        return None
