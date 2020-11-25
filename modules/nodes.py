import v_nodes


class node:

    def __init__(self, node_name):
        self.name = node_name

    def delete(self):
        return v_nodes.delete_node()

    def get_param(param_name):
        return ""


def create_node(node_name):
    return v_nodes.create_node(node_name)


def get_node(node_name):

    _node = node(node_name)

    return _node
