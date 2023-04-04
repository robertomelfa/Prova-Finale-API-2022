#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct s_Tree {
    char* string;
    struct s_Tree *left, *right;
};
typedef struct s_Tree Tree;
typedef Tree* pTree;

struct s_node{
    char* string;
    struct s_node* next;
};
typedef struct s_node node;
typedef node* pNode;

struct s_constraints{       // struct of the constraint
    char letter;        // letter
    char constraint;    // type of constraint
    int count;          // how many letters in the string
    int pos;            // position of the letter
    struct s_constraints* next;
};
typedef struct s_constraints constraints;
typedef constraints* pconstraints;

char* duplicate_string(char* string, int k){
    if(string != NULL){
        char* temp=NULL;
        temp = malloc(k+1);
        return strncpy(temp, string, k+1);
    }
    return NULL;
}

void rb_insert(pTree* A, pTree z){
    pTree y, x;
    y = NULL;
    x = *A;
    while(x != NULL){
        y=x;
        if(strcmp(z->string, x->string) < 0){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    if(y == NULL){
        *A = z;
    }else if(strcmp(z->string, y->string) < 0){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left = NULL;
    z->right = NULL;
}

pTree input(char* word, int k){ //I take the first words as input, at +nuova_word I stop
    pTree A;
    A = NULL;
    if(scanf("%s", word) > 0){
        while(strcmp(word, "+nuova_partita")!=0){
            if(strcmp(word, "+insert_inizio") !=0 && strcmp(word, "+insert_fine")!=0){
                pTree z;
                z = malloc(sizeof(Tree));
                z -> string = duplicate_string(word, k);        // duplicatete the input word
                rb_insert(&A, z);
            }
            if(scanf("%s", word) > 0){}
        }
    }
    return A;
} // complexity number of admissible words


// filters strings that match the constraints
int filter_string(char* string, pconstraints currentC, int k){
    if(currentC != NULL){
        int delete, count, i, p;
        while(currentC != NULL){
            i = currentC->pos;
            delete = 0;
            count = 0;
            if(currentC->constraint == '+' && *(string+i) != currentC->letter){
                delete = 1;
            }else if(currentC->constraint == '|'){
                delete = 1;
                if(*(string+i)!=currentC->letter){
                    for(p=0; p<k; p++){
                        if(*(string+p)==currentC->letter && p!=i){
                            delete =0;
                            count ++;
                        }
                    }
                }
                if(count < currentC->count){
                    delete = 1;
                }
            }else if(currentC->constraint == '/'){
                if(*(string+i) == currentC->letter){
                    delete = 1;
                }else{
                    for(p=0; p<k; p++){
                        if(*(string+p) == currentC->letter){
                            count ++;
                        }
                    }
                    if(count != currentC->count){
                        delete = 1;
                    }
                }
            }
            if(delete == 1){
                return 1;
            }
            currentC = currentC->next;
        }
    }
    return 0;
}

void deleteTree(pTree A){
    if(A != NULL){
        if(A->left == NULL && A->right == NULL){
            free(A->string);
            free(A);
        }else{
            deleteTree(A->left);
            deleteTree(A->right);
            free(A->string);
            free(A);
        }
    }
    A=NULL;
}

void printTree(pTree A){
    if (A != NULL) {
        printTree(A->left);
        printf("%s\n", A->string);
        printTree(A->right);
    }
}

// delete elements from constraints
void deleteConstraint(pconstraints * head){
    pconstraints elem;
    if(*head != NULL){
        while(*head != NULL){
            elem = *head;
            *head = elem->next;
            free(elem);
        }
    }
    head = NULL;
} // complexity = number of nodes

// insert constraint in the list
void insert_constraint(pconstraints * head, char letter, char constraint, int count, int pos){
    pconstraints elem;
    elem = NULL;
    elem = malloc(sizeof(constraints));
    if(elem != NULL){
        if(head == NULL){
            elem->next = NULL;
            *head = elem;
        }else{
            elem->next = *head;
            *head = elem;
        }
        elem->letter = letter;
        elem->constraint = constraint;
        elem->count = count;
        elem->pos = pos;
    }
} // constant complexity

// compare the permissible words
int comparison_elegible(pTree x, char* string1){
    while(x != NULL){
        if(strcmp(string1, x->string) < 0){
            x = x->left;
        }else if(strcmp(string1, x->string) > 0){
            x = x->right;
        }else{
            if(strcmp(string1, x->string) == 0){
                return 0;
            }
        }
    }
    printf("not_exists\n");
    return 1;
} // complexity number of admissible words * k

// filter the strings in the list of permissible words
int filter(pNode* head, pconstraints constraint, int k, int num){
    pNode current = NULL, prev = NULL, elem = NULL;
    current = *head;
    while(current!=NULL){
        if(filter_string(current->string, constraint, k) == 1){
            num --;
            if(prev == NULL){
                *head = current->next;
                elem = current;
                current = *head;
                free(elem);
            }else{
                if(current->next != NULL){
                    prev->next = current->next;
                    elem = current;
                    current = current->next;
                    free(elem);
                }else{
                    prev->next = NULL;
                    elem = current;
                    current = prev;
                    free(elem);
                }
            }
        }else{
            prev = current;
            current = current->next;
        }
    }
    return num; // num of strings left
}

//
void split(pNode head, pNode* list1, pNode* list2){
    pNode current = NULL, next = NULL;
    current = head;
    next = current->next;
    while(next != NULL){
        next = next->next;
        if(next != NULL){
            current = current->next;
            next = next->next;
        }
    }
    *list1 = head;
    *list1 = current->next;
    current->next = NULL;
}

pNode merge(pNode list1, pNode list2){
    pNode head;
    head = NULL;
    if(list1 == NULL){
        return list2;
    }else if(list2 == NULL){
        return list1;
    }
    if(strcmp(list1->string, list2->string) < 0){
        head = list1;
        head->next = merge(list1->next, list2);
    }else{
        head = list2;
        head->next = merge(list1, list2->next);
    }
    return head;
}

void mergeSort(pNode* head){
    pNode current = NULL, list1 = NULL, list2 = NULL;
    current = *head;
    if(current != NULL && current->next != NULL){
        split(current, &list1, &list2);
        mergeSort(&list1);
        mergeSort(&list2);
        *head = merge(list1, list2);
    }
}

void emptylist(pNode current){
    if(current != NULL){
        pNode elem = NULL;
        while(current != NULL){
            elem = current;
            current = current->next;
            free(elem);
        }
    }
    current=NULL;
} // complexity = num of nodes

void printlist(pNode head){
    if(head != NULL){
        while(head != NULL){
            printf("%s\n", head->string);
            head = head->next;
        }
    }
} // complexity = num of nodes

void insertlist(pNode* head, char* string){
    pNode elemento;
    elemento = malloc(sizeof(node));
    elemento->string = string;
    if(*head == NULL){
        *head = elemento;
        elemento->next = NULL;
    }else{
        elemento->next = *head;
        *head = elemento;
    }
}

void Treelist(pTree A, pNode* list, pconstraints constraint, int k){
    if (A != NULL){
        Treelist(A->right, list, constraint, k);
        if(filter_string(A->string, constraint, k) == 0){
            insertlist(list, A->string);
        }
        Treelist(A->left, list, constraint, k);
    }
}

int num_words(pNode head){
    int i=0;
    if(head != NULL){
        while(head != NULL){
            head = head->next;
            i++;
        }
    }
    return i;
}   // complexity = num of permessible words

// comparison between the words of the list and the references word
int comparison_reference(pconstraints* constraint, char* string1, char* string2, int k){// p = r
    if(strcmp(string1, string2) == 0){
        printf("ok\n");
        return 0;
    }else{
        int sentinel, count, istance, same_characters;
        for(int i = 0; i < k; i++){
            count = 0;
            sentinel = 0;
            istance = 0;
            same_characters = 0;
            if(string1[i] == string2[i]){
                printf("+");
                insert_constraint(constraint, string2[i], '+', 1, i);
            }else{
                for(int p = 0; p < k; p++){
                    if(string2[p] == string2[i]){
                        sentinel = 1;
                        break;
                    }
                }
                if(sentinel == 0){
                    printf("/");
                    insert_constraint(constraint, string2[i], '/', 0, i);
                }else{
                    for(int p = 0; p < k; p++){
                        if(string1[p] == string2[i]){
                            count ++;
                        }
                        if(string2[p] == string1[p] && string2[p] == string2[i]){
                            istance ++;
                        }
                        if(string2[p] != string1[p] && string2[p] == string2[i] && p < i){
                            same_characters ++;
                        }
                    }
                    if(same_characters >= count-istance){
                        sentinel = 0;
                        printf("/");
                        insert_constraint(constraint, string2[i], '/', count, i);
                    }
                    if(sentinel == 1){
                        printf("|");
                        insert_constraint(constraint, string2[i], '|', istance+same_characters+1, i);
                    }
                }
            }
        }
        printf("\n");
    }
    return 1;
} // complexity = k^2

// manages the current game
void match(pTree A, int k, int n, char* r, char* word){
    int i = 0, ko = 1, num = 0, order = 0;
    pNode head;
    head = NULL;
    pconstraints constraint;
    constraint = NULL;
    while(i < n || !feof(stdin)){
        if(scanf("%s", word)>0){}
        if(feof(stdin) || (i==n)){
            break;
        }else{
            if(strcmp(word, "+nuova_partita") == 0){
                break;
            }else if(strcmp(word, "+insert_inizio") == 0){
                if(scanf("%s", word)>0){}
                while(strcmp(word, "+insert_fine") != 0){
                    pTree z;
                    z = malloc(sizeof(Tree));
                    z->string = duplicate_string(word, k);
                    rb_insert(&A, z);
                    if(filter_string(word, constraint, k) == 0 && i!=0){
                        insertlist(&head, z->string);
                        order = 1;
                        num ++;
                    }
                    if(scanf("%s", word) > 0){}
                }
            }else if(strcmp(word, "+stampa_filtrate") == 0){
                if(i == 0){
                    printTree(A);
                }else{
                    if(order == 1){
                        mergeSort(&head);
                    }
                    printlist(head);
                }
            }else{
                if(comparison_elegible(A, word) == 0){
                    if(comparison_reference(&constraint, r, word, k) == 0){
                        char ok[k];
                        strcpy(ok, word);
                        if(scanf("%s", word)>0){}
                        if(strcmp(word, "+stampa_filtrate") == 0){
                            printf("%s\n", word);
                        }
                        ko = 0;
                        break;
                    }else{
                        if(i==0){
                            Treelist(A, &head, constraint, k);
                            num = num_words(head);
                        }else{
                            num = filter(&head, constraint, k, num);
                        }
                        printf("%d\n", num);
                    }
                    i++;
                }
            }
        }
    }
    if(ko == 1){
        printf("ko\n");
    }
    if(i == n){
        if(strcmp(word, "+stampa_filtrate")==0){
            if(i == 0){
                printTree(A);
            }else{
                printlist(head);
            }
        }
    }
    emptylist(head);
    deleteConstraint(&constraint);
} // complexity = n * eligible * constraints * k^2 + n * filter_words^2 * k^2

int main(){
    int k = 0, n = 0;
    pTree A;
    A=NULL;
    if(scanf("%d", &k)>0){}
    char* r;
    r=malloc(k+1);
    char* command;
    if(k>17){
        command = malloc(k+1);
    }else{
        command = malloc(18);
    }
    A=input(command, k);
    while(!feof(stdin)){
        if(strcmp(command, "+nuova_partita") == 0){
            if(scanf("%s", r)>0){}
            if(scanf("%d", &n)>0){}
            match(A, k, n, r, command);
        }else if(strcmp(command, "+insert_inizio") == 0){
            if(scanf("%s", command)>0){}
            while(strcmp(command, "+insert_fine") != 0){
                pTree z;
                z=malloc(sizeof(Tree));
                z->string = duplicate_string(command, k);
                rb_insert(&A, z);
                if(scanf("%s", command)>0){}
            }
        }else{
            if(scanf("%s", command)>0){}
        }
    }
    deleteTree(A);
    free(command);
    free(r);
    return 0;
} // complexity = (n * eligible * constraints * k + n * filter_words^2 * k^2) * num of matches

