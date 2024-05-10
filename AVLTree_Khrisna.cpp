#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int balance_factor(AVLNode *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

AVLNode *new_node(int key) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode *right_rotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode *left_rotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode *insert_node(AVLNode *node, int key) {
    if (node == NULL)
        return new_node(key);

    if (key < node->key)
        node->left = insert_node(node->left, key);
    else if (key > node->key)
        node->right = insert_node(node->right, key);
    else // Duplicate keys not allowed
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = balance_factor(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return right_rotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return left_rotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

AVLNode *min_value_node(AVLNode *node) {
    AVLNode *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode *delete_node(AVLNode *root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = delete_node(root->left, key);
    else if (key > root->key)
        root->right = delete_node(root->right, key);
    else {
        if (root->left == NULL || root->right == NULL) {
            AVLNode *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVLNode *temp = min_value_node(root->right);
            root->key = temp->key;
            root->right = delete_node(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = balance_factor(root);

    if (balance > 1 && balance_factor(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && balance_factor(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && balance_factor(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && balance_factor(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

AVLNode* search(AVLNode* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (root->key < key)
        return search(root->right, key);
    return search(root->left, key);
}

void inorder_traversal(AVLNode *root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        printf("%d ", root->key);
        inorder_traversal(root->right);
    }
}

int main(){
    AVLNode *root = NULL;
	root = insert_node(root, 10);
    root = insert_node(root, 15); 
    root = insert_node(root, 20); 
    root = insert_node(root, 9);
    root = insert_node(root, 5);
    root = insert_node(root, 16);
    root = insert_node(root, 17);
    root = insert_node(root, 8);
    root = insert_node(root, 6);

    printf("Before deletion: ");
    inorder_traversal(root);
    printf("\n");

    int value_to_delete = 15;
    root = delete_node(root, value_to_delete);
    printf("After deletion of %d: ", value_to_delete);
    inorder_traversal(root);
    printf("\n");

    int value_to_search = 17;
    AVLNode* result = search(root, value_to_search);
    if (result != NULL)
        printf("%d found in the tree.\n", value_to_search);
    else
        printf("%d not found in the tree.\n", value_to_search);

    return 0;
}

