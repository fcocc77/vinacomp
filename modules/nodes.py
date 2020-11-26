import v_nodes


class node:

    def __init__(self, node_name):
        self.name = node_name

    def delete(self):
        return v_nodes.delete_node()

    def set_position(self, x, y):
        return v_nodes.set_position(self.name, x, y)

    def get_param(self, param_name):
        return ""


def create_node(node_name):
    name = v_nodes.create_node(node_name)

    return get_node(name)


def get_node(node_name):
    return node(node_name)
