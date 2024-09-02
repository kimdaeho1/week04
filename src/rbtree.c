#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); //메모리의 블록할당하고, 0으로 초기화. rbtree*은 calloc가 반환하는 void*타입의 포인터를rbtree*형으로 변환.
  // TODO: initialize struct if needed
  // p라는 트리에 동적 할당을 한다.
  node_t *nil = (node_t *)calloc(1, sizeof(node_t)); //
  //닐 노드에도 동적 할당을 해준다
  nil->color = RBTREE_BLACK; //닐노드의 색깔은 RBTREE_BLACK으로 설정
                            //매 리프 노드에 닐도르를 각각 달아주게 되면, 트리의 리프 노드가 500개면 닐노드는 1000개다! 
                            //와따 많다 메모리 많이도 들겠다 그래서 nil노드를 써서 모든 트리의 리프 노드가 nil노드를 가리키게.(물론 nil노드가 리프 노드지만, 사실 없는 값이라고 생각하며 리프노드를 직접 가리키는 노드들이 리프 노드라고 생각하였다.)
  p->nil = nil;  //닐노드의 포인터는 root의 값을 가리키고, 그걸 nil노드로 변경
  p->root = p->nil ; //text rbtree에서는, SENTINEL이 정의되지 않았을 때, t->root 가 NULL로 갔는데, calloc를 설정하면 데이터가 0이어서 알아서 NULL이 된답니다.
  
  return p;
}

void recursion_rbtree(rbtree *t, node_t *node)
{
  if (node->left != t->nil)
  recursion_rbtree(t, node->left);
  if (node->right != t->nil)
  recursion_rbtree(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  //트리노드의 메모리를 뺴야해요. 그냥 rbtree의 자체 메모리를 뺴버리란 소린가?
  node_t *node = t->root;
  if (node != t->nil)
  {
    recursion_rbtree(t, node); //재귀함수로 트리의 값들에 메모리 할당을 전부 지워주기
  }
  free(t->nil);
  free(t);

}

rotate_left(rbtree *t, node_t *target){
  //왼쪽 회전, 내가 올라가고, 나의 부모가 내 왼쪽 자식이 되게 하는 것.
  node_t*next = target->right;  //부모의 오른쪽자식이 부모가 되게 하기
  target->right = next->left; //내 왼쪽 자식이 부모의 오른쪽 자식

  if (next->left != t->nil){
    next->left->parent = target;
  }
  next->parent = target->parent;

  if (target->parent == t->nil){
    t->root = next;
  } else if (target == target->parent->right){
    target->parent->right = next;
  } else {
    target->parent->left = next;
  }
  next->left = target;
  target->parent = next;

}



rotate_right(rbtree *t, node_t *target){   
  //오른쪽 회전, 내가 올라가고, 나의 부모가 내 오른쪽 자식이 되게 하는것.
  node_t*next = target->left; //타겟의 왼쪽 자식이 next가 된다.
  target->left = next->right; //next의 오른쪽 자식이 타겟의 왼쪽 자식이 된다.

  if (next->right != t->nil){ //next오른쪽 자식이 닐노드가 아니면
    next->right->parent = target; //next의 오른쪽 자식의 부모가 target
  }
  next->parent = target->parent; //next의 부모는 target의 부모가 된다

  if(target->parent == t->nil) { //만약 target이 루트노드라면
    t->root = next; //응 이제 내가 루트노드야
  } else if (target == target->parent->right) { //target이 조부모의 오른쪽자녀였다면
    target->parent->right = next; //이제 내가 오른쪽자녀임
  } else {  //왼쪽이면
    target->parent->left = next; //내가 왼쪽 자녀임
  }
  next->right = target; //부모가 이제 내 오른쪽 자녀고
  target->parent = next;  //target의 부모는 내가 되는거임
}

void check_and_correct(rbtree *t, node_t *r) {
    while (r->parent->color == RBTREE_RED) { // 부모가 빨간색인 동안 루프 실행
        node_t *p = r->parent;  // 부모노드
        node_t *gp = p->parent; // 조부모 노드

        if (p == gp->left) { // 부모가 조부모의 왼쪽 자녀일 때
            node_t *uncle = gp->right; // 삼촌 노드

            if (uncle->color == RBTREE_RED) { // 삼촌이 빨간색인 경우
                p->color = RBTREE_BLACK;      //부모를 블랙
                uncle->color = RBTREE_BLACK;  //삼촌을 블랙
                gp->color = RBTREE_RED;       //조부모와 그 자녀들의 색을 바꾼후
                r = gp;                       // 루프를 계속해서 위로 이동
            } else {
                if (r == p->right) { // 노드가 부모의 오른쪽 자녀인 경우
                    r = p;            // 부모가 target이 되서 회전해야 하므로
                    rotate_left(t, r); // 왼쪽 회전
                }
                r->parent->color = RBTREE_BLACK; //그리고 부모의 색깔과
                gp->color = RBTREE_RED;         //조부의 색깔을 바꾸고
                rotate_right(t, gp); // 오른쪽 회전
            }
        } else { // 부모가 조부모의 오른쪽 자녀일 때
            node_t *uncle = gp->left; // 삼촌 노드

            if (uncle->color == RBTREE_RED) { // 삼촌이 빨간색인 경우
                p->color = RBTREE_BLACK;      //아까랑 똑같이~
                uncle->color = RBTREE_BLACK;
                gp->color = RBTREE_RED;
                r = gp; // 루프를 계속해서 위로 이동
            } else {
                if (r == p->left) { // 노드가 부모의 왼쪽 자녀인 경우
                    r = p;
                    rotate_right(t, r); // 오른쪽 회전
                }
                r->parent->color = RBTREE_BLACK;
                gp->color = RBTREE_RED;
                rotate_left(t, gp); // 왼쪽 회전
            }
        }
    }
    t->root->color = RBTREE_BLACK; // 루트 노드는 항상 검은색으로 설정
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // 자 내가 숫자를 입력하면, 만약 root가 비어있다면 그 친구가 root가 된다. 
  // root가 비어있지 않다면, 그 친구부터 root의 key값과 비교한다.
  // 자 추가할라면 새로운 노드부터 추가합시다
  //새로운 노드 설정
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t->nil;

  node_t *y = t->nil; //부모노드
  node_t *x = t->root; //트리의 현재 루트 노드

  while (x != t->nil) {   //루트노드가 nil을 가리키기 않을 떄까지. 그니까 y를 x로 설정하고,
  y=x;
  if(new_node->key < x->key) {
    x=x->left;
  } else {
    x = x->right;
  }
  }
  new_node ->parent = y;
  if (y == t->nil) {
    t->root = new_node;
    new_node->parent = t->nil; //부모가 없는 루트 노드의 부모를 nil로 설정
  } else if (new_node->key < y->key) {
    y->left = new_node;
  } else {
    y->right = new_node;
  }
  
  check_and_correct(t, new_node); //RB트리의 조건을 검색하고, 맞는지 판단하는 함수.
  
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while ( x != t->nil) {
    if (x->key == key) {
      return x;
    } else if (x->key > key) {
      x = x->left;
    } else {
      x = x->right;
    } 
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t*x = t->root;

  if(x==t->nil) {
    return NULL;
  }

  while(x->left!= t->nil){
  x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t*x = t->root;

  if(x==t->nil) {
    return NULL;
  }

  while(x->right != t->nil) {
    x = x->right;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // p가 지우는 target이겠지?
  // 만약 p의 노드의 자녀가 없거나 하나라면, 삭제되는 색 = 삭제되는 노드의 색
  // p의 노드의 자녀가 둘다 있다면, 삭제되는 색은 삭제되는 노드의 오가수의 색.(오른쪽 subtree에서 가장 작은 값)
  node_t*replace; //replace포인터 선언, 교체할 노드
  node_t *successor; //successor저장
  color_t original_color = p->color; //삭제할 노드의 원래 색깔

  if (p->left == t->nil) { //왼쪽에 자식이 없을경우
    replace = p->right;
    transplant(t, p, replace); //p를 지울거니까
  } else if(p->right == t->nil) {//오른쪽에 자식이 없을경우
    replace = p->left;
    transplant(t, p, replace); //p를 replace로 바꿀거야
  } else {  //자식이 둘다 있으면
    successor = rbtree_min(p->right);
    original_color = successor->color;
    if (successor != p->right) {  //successor가 오른쪽 아이가 아닐경우
      transplant(t, successor, successor->right); //successor와 successor의 자식의 위치를 바꾸고
      successor->right = p->right; // successor의 오른쪽이 p의 오른쪽이 되게하고
      successor->right->parent = successor; //successor를 바꿔야되는 위치, 삭제한 노드의 위치로 복귀.
    } 
    transplant(t, p, successor);
    successor->left = p->left;
    successor->left->parent = successor;
    original_color = p->color;   
  }
  if (original_color == RBTREE_BLACK) {
  check_erase_correct(t, replace);
  }
  
  free(p);
  return 0;
}

void transplant (rbtree *t, node_t*del, node_t*replace) {
  if (del->parent == t->nil) {
    t->root = replace;
  } else if (del == del->parent->left) {
    del->parent->left = replace;
  } else {
    del->parent->left = replace;
  } replace->parent = del->parent;
}

void check_erase_correct(rbtree *t, node_t *target) {
  node_t *uncle;
  while ((target != t->root) && (target->color == RBTREE_BLACK)) {
    if (target == target->parent->left) {
      uncle = target->parent->right;
      if (uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        rotate_left(t, target->parent);
        uncle = target->parent->right;
      } 
      if (uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        if (uncle->right->color == RBTREE_BLACK) {
          uncle->left->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rotate_right(t, uncle);
          uncle = target->parent->right;
        }
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->right->color = RBTREE_BLACK;
        rotate_left(t, target->parent);
        target = t->root;
      }
    }
    else {
      uncle = target->parent->left;
      if (uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        rotate_right(t, target->parent);
        uncle = target->parent->left;
      }
      if (uncle->right->color == RBTREE_BLACK && uncle->left->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        if (uncle->left->color == RBTREE_BLACK) {
          uncle->right->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rotate_left(t, uncle);
          uncle = target->parent->left;
        }
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->left->color = RBTREE_BLACK;
        rotate_right(t, target->parent);
        target = t->root;
      }
    }
  }  
  target->color = RBTREE_BLACK;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

