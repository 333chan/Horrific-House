#include "Node.h"

Node::Node(Vector3 initPos)
{
    pos = initPos;
}

Node::~Node()
{
}

//ƒm[ƒhŠÖŒW
void Node::Reset(void)
{
    cost = -1;
    toGoal = nullptr;
}

void Node::RemoveConnect(SharedNode node)
{
    for (auto it = connectNode.begin(); it != connectNode.end(); it++)
    {
        if (it->node == node) {
            it = connectNode.erase(it);
            break;
        }
    }
}

