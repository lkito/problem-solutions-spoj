#include <iostream>
using namespace std;

struct tree{
    tree *l,*r;
    int flowCount;
    int horCount;
    tree(){
        l = NULL; r = NULL; horCount = 0; flowCount = 0;
    }
};

void updateTree(tree*& root, int querLeft, int querRight, int left, int right){
    if(left > querRight || right < querLeft) return;
    if(root == NULL) root = new tree();
    if(left >= querLeft && right <= querRight) {
        root->horCount++;
        return;
    }
    updateTree(root->l, querLeft, querRight, left, (left + right)/2);
    
    updateTree(root->r, querLeft, querRight, (left + right)/2 + 1, right);
}

int getFlow(tree*& root, int ind, int left, int right, int curHors){
    if(root == NULL) root = new tree();
    curHors += root->horCount;
    if(right == left){
        int result = curHors - root->flowCount;
        root->flowCount = curHors;
        return result;
    }
    if(ind <= (left + right)/2){
        return getFlow(root->l, ind, left, (left + right)/2, curHors);
    } else  return getFlow(root->r, ind, (left + right)/2 + 1, right, curHors);
}

int main(int argc, char **argv){
    ios::sync_with_stdio(0);
    int n, a, b, answ;
    cin >> n;
    tree* root = NULL;
    int answAr[n];
    for(int i = 0; i < n; i++){
        answ = 0;
        cin >> a >> b;
        answ += getFlow(root, a, 1, 100001, 0);
        answ += getFlow(root, b, 1, 100001, 0);
        answAr[i] = answ;
        if(b - a > 1) updateTree(root, a + 1, b - 1, 1, 100001);
    }
    for(int k = 0; k < n; k++){
        cout << answAr[k] << endl;
    }

    return 0;
}
