/*
I,Weidi MIN, am submitting the assignment for
an individual project.
I declare that the assignment here submitted is original except for
source material explicitly acknowledged, the piece of work, or a part
of the piece of work has not been submitted for more than one purpose
(i.e. to satisfy the requirements in two different courses) without
declaration. I also acknowledge that I am aware of University policy
and regulations on honesty in academic work, and of the disciplinary
guidelines and procedures applicable to breaches of such policy and
regulations, as contained in the University website
http://www.cuhk.edu.hk/policy/academichonesty/.
It is also understood that assignments without a properly signed
declaration by the student concerned will not be graded by the
teacher(s).
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct AVLTreeNode{
    int data;
    int height;
    struct AVLTreeNode *left;
    struct AVLTreeNode *right;
};

struct AVLTreeNode *createNode(int x) {
    struct AVLTreeNode *node = (struct AVLTreeNode *)malloc(sizeof(struct AVLTreeNode));
    node->data = x;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct AVLTreeNode * Lchild(struct AVLTreeNode * root) {
    return root->left;
}

struct AVLTreeNode * Rchild(struct AVLTreeNode * root) {
    return root->right;
}

int Data(struct AVLTreeNode * root) {
    return root->data;
}

bool isEmpty(struct AVLTreeNode * root) {
    return root == NULL;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

int Height(struct AVLTreeNode * root) {
    if (isEmpty(root)) {
        return 0;
    }
    return 1+max(Height(root->left), Height(root->right));
}

// left-left imbalance
struct AVLTreeNode *llRotation(struct AVLTreeNode *y) {
    // write your code here
    struct AVLTreeNode *x = y->left;
	struct AVLTreeNode *tmp = x->right;
	x->right = y;
	y->left = tmp;

	y->height = Height(y);
	x->height = Height(x);
	return x;
}

// right-right imbalance
struct AVLTreeNode *rrRotation(struct AVLTreeNode *x) {
    // write your code here
    struct AVLTreeNode * y= x->right;
	struct AVLTreeNode * tmp= y->left;
	y->left =x;
	x->right=tmp;
	
	x->height =Height(x);
	y->height =Height(y);
	return y;
}

// left-right imbalance
struct AVLTreeNode *lrRotation(struct AVLTreeNode *y) {
    // write your code here
    y->left =  rrRotation(y->left);
    return llRotation(y);
}

// right-left imbalance
struct AVLTreeNode *rlRotation(struct AVLTreeNode *x) {
    // write your code here
   	x->right=llRotation(x->right);
   	return rrRotation(x);
}

int balanceFactor(struct AVLTreeNode *root)
{
    if (root == NULL) {
        return 0;
    }
    return Height(root->left) - Height(root->right);
}

struct AVLTreeNode * insertNode(struct AVLTreeNode * root, int x) {
    // write your code here
   if (root == NULL) return(createNode(x)); 
    if (x < root->data)
        root->left  = insertNode(root->left, x);
    else if (x > root->data)
        root->right = insertNode(root->right, x);
    else  return root;
 
    root->height = Height(root);
 
    int balance = balanceFactor(root);
 
    if (balance > 1 && x < root->left->data)
        return llRotation(root);

    if (balance < -1 && x > root->right->data)
        return rrRotation(root);
 
    // Left Right Case
    if (balance > 1 && x> root->left->data)
    {
        root->left =  rrRotation(root->left);
        return llRotation(root);
    }
 
    // Right Left Case
    if (balance < -1 && x < root->right->data)
    {
        root->right = llRotation(root->right);
        return rrRotation(root);
    }
 
    /* return the (unchanged) node pointer */
    return root;
}

struct AVLTreeNode* deleteNode(struct AVLTreeNode* root, int x) {
    // write your code here
	struct AVLTreeNode * curr = root;
    struct AVLTreeNode * prev = NULL;
 
    // Check if the x is actually present in the BST. the variable prev points to the parent of the x to be deleted.
    while (curr != NULL && Data(curr) != x) 
    {
        prev = curr;
        if (x < Data(curr))
            curr = Lchild(curr);
        else
            curr = Rchild(curr);
    }
 
    if (curr == NULL) 
        return root;
    else if (Lchild(curr) == NULL || Rchild(curr) == NULL) 
    {
        // Node newcurr will replace the node to be deleted.
        struct AVLTreeNode * newcurr;
 
        // If the left child does not exist.
        if (Lchild(curr) == NULL)
            newcurr = Rchild(curr);
        else
            newcurr = Lchild(curr);
        // check if the node to be deleted is the root.
        if (prev == NULL)
            return newcurr;
 
        // Check if the node to be deleted is prev's left or right child and then replace this with newcurr.
        if (curr == Lchild(prev))
            prev->left = newcurr;
        else
            prev->right = newcurr;
        // Free memory of the node to be deleted.
        free(curr);
    }
 
    // Node to be deleted has two children.
    else {
        struct AVLTreeNode * p = NULL;
        struct AVLTreeNode * temp;
 
        // Compute the inorder successor.
        temp = Lchild(curr);
        while (Rchild(temp) != NULL) {
            p = temp;
            temp = Rchild(temp);
        }
 		//in bst, we can use successor or predessor it depends which one we use
        // Check if the parent of the successor is the curr or not(i.e. curr = the node which has the same data as the given data by the user to be deleted). if it isn't, then make the left child of its parent equal to the successor's right child.
        if (p != NULL)
        {
            p->right = Lchild(temp);
        }
        // If the inorder successor was the curr (i.e. curr = the node which has the same data as the given data by the user to be deleted), then make the right child of the node to be deleted equal to the right child of the successor.
        else
        {
            curr->left = Lchild(temp); 
        }
        curr->data = Data(temp);
        free(temp);
    }
  //  return root; CARE!!!!!

	root->height = Height(root);
 
    int balance = balanceFactor(root);
 
    if (balance > 1 && balanceFactor(root->left)>=0)
        return llRotation(root);

    if (balance < -1 && balanceFactor(root->right)<=0)
        return rrRotation(root);
 
    // Left Right Case
    if (balance > 1 && balanceFactor(root->left)<0)
    {
        root->left =  rrRotation(root->left);
        return llRotation(root);
    }
 
    // Right Left Case
    if (balance < -1 && balanceFactor(root->right)>0)
    {
        root->right = llRotation(root->right);
        return rrRotation(root);
    }
 
    return root;
}

void Preorder(struct AVLTreeNode *root) {
    if (!isEmpty(root)) {
        printf("%d ", Data(root));
        Preorder(Lchild(root));
        Preorder(Rchild(root));
    }
}

void Inorder(struct AVLTreeNode *root) {
    if (!isEmpty(root)) {
        Inorder(Lchild(root));
        printf("%d ", Data(root));
        Inorder(Rchild(root));
    }
}

void Postorder(struct AVLTreeNode *root) {
    if (!isEmpty(root)) {
        Postorder(Lchild(root));
        Postorder(Rchild(root));
        printf("%d ", Data(root));
    }
}

int main() {
    int n, m, x;
    struct AVLTreeNode * root = NULL;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        root = insertNode(root, x);
    }
    printf("Preorder: ");
    Preorder(root);
    printf("\nInorder: ");
    Inorder(root);
    printf("\nPostorder: ");
    Postorder(root);
    printf("\n");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        root = deleteNode(root, x);
    }
    printf("Preorder: ");
    Preorder(root);
    printf("\nInorder: ");
    Inorder(root);
    printf("\nPostorder: ");
    Postorder(root);
    printf("\n");
    return 0;
}
