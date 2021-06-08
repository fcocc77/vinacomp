import __py_nodes__ #type: ignore
import __py_node_graph__ #type: ignore
import params


class node_graph:
    def __init__(self, node_graph_name):
        self.name = node_graph_name

    def get_nodes(self) -> list:
        return __py_node_graph__.get_nodes_list(self.name)

    def create_node(self, node_id: str, basic_creation: bool = True):
        node_name: str = __py_node_graph__.create_node(self.name, node_id, basic_creation)
        return get_node(node_name)

    def get_name(self) -> str:
        return self.name


class node:

    def __init__(self, node_name):
        self.name = node_name

    def delete(self):
        return __py_nodes__.delete_node(self.name)

    def set_position(self, x, y):
        return __py_nodes__.set_position(self.name, x, y)

    def get_param(self, param_name):
        return params.get_param(self.name, param_name)

    def get_name(self):
        return self.name

    def rename(self, name):
        __py_nodes__.rename(self.name, name)
        self.name = name

    def get_node_graph(self):
        node_graph_name: str = __py_nodes__.get_node_graph_name(self.name);
        return get_node_graph(node_graph_name)


def get_node_graph(node_graph_name: str):
    if __py_node_graph__.node_graph_exists(node_graph_name):
        return node_graph(node_graph_name)
    else:
        print('Error: The \'' + node_graph_name + '\' node_graph does not exist.')
        return None

def create_node(node_name):
    name = __py_nodes__.create_node(node_name)
    return get_node(name)


def get_node(node_name):
    if __py_nodes__.node_exists(node_name):
        return node(node_name)
    else:
        print('Error: The \'' + node_name + '\' node does not exist.')
        return None
