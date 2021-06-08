import __py_node_graph__ #type: ignore

class node_graph:
    def __init__(self, node_graph_name):
        self.name = node_graph_name

    def get_nodes(self) -> list:
        return __py_node_graph__.get_nodes_list(self.name)

    def create_node(self, node_id: str, basic_creation: bool = True) -> bool:
        return __py_node_graph__.create_node(self.name, node_id, basic_creation)

    def get_name(self) -> str:
        return self.name


def get_node_graph(node_graph_name: str):
    if __py_node_graph__.node_graph_exists(node_graph_name):
        return node_graph(node_graph_name)
    else:
        print('Error: The \'' + node_graph_name + '\' node_graph does not exist.')
        return None

