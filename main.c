/* GENERATİNG BACKUS-NAUR FORM TREE PROGRAM */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct haveNoChild{
    char *data;
};
struct haveOneChild{
    struct haveNoChild *center;
};
struct haveTwoChildren{
    struct haveOneChild *left;
    void *right;
    int degreeOfRightChild;
};
struct haveThreeChildren{
    void *left;
    struct haveOneChild *center;
    void *right;
    int degreeOfLeftChild;
    int degreeOfRightChild;
};
typedef struct files{
    char **op; int sizeOp;
    char **preOp; int sizePreOp;
    char **relOp; int sizeRelOp;
    char **setOp; int sizeSetOp;
    char **var; int SizeVar;
}Files;
typedef struct haveNoChild NoChild;
typedef struct haveOneChild OneChild;
typedef struct haveTwoChildren TwoChildren;
typedef struct haveThreeChildren ThreeChildren;
int readFiles(char *fileName,char ***storage);
int randInt(int lower,int upper);
void createForHaveThreeChildren(ThreeChildren *cond,int rand,int randLeft,int randRight,Files *files);
void createForHaveTwoChildren(TwoChildren *cond,int rand,Files *files);
void traverseForHaveThreeChildren(ThreeChildren *cond);
void traverseForHaveTwoChildren(TwoChildren *cond);
int main(int argc,char *argv[]){
    srand(time(NULL));
    Files files={(char **)calloc(1,sizeof(char *)),readFiles("op",&(files.op)),
                 (char **)calloc(1,sizeof(char *)),readFiles("pre_op",&(files.preOp)),
                 (char **)calloc(1,sizeof(char *)),readFiles("rel_op",&(files.relOp)),
                 (char **)calloc(1,sizeof(char *)),readFiles("set_op",&(files.setOp)),
                 (char **)calloc(1,sizeof(char *)),readFiles("var",&(files.var))};
    ThreeChildren *cond=(ThreeChildren *)calloc(1,sizeof(ThreeChildren));
    createForHaveThreeChildren(cond,randInt(1,2),randInt(3,5),randInt(3,5),&files);
    printf("if"); 
    traverseForHaveThreeChildren(cond); 
    printf(" {}");
    return 0;
}
int readFiles(char *fileName,char ***storage){
    FILE *file=fopen(fileName,"r");
    fseek(file,0,SEEK_END);
    int sizeFile=ftell(file);
    rewind(file);
    int size=0;
    while(!feof(file)){
        (*(storage))[size]=(char *)calloc(sizeFile+1,sizeof(char));
        fscanf(file,"%s",(*(storage))[size]);
        (*(storage))[size]=realloc((*(storage))[size],(strlen((*(storage))[size])+1)*sizeof(char));
        *(storage)=realloc(*(storage),sizeof(char *)*(size+2));
        if(strcmp((*(storage))[size],"")!=0) size++;
    }
    fclose(file);
    return size;
}
int randInt(int lower,int upper){
    int random=rand();
    return (random%(upper-lower+1))+lower;
}
void createForHaveThreeChildren(ThreeChildren *cond,int rand,int randLeft,int randRight,Files *files){
    int flag=0;
    if(rand==1){ /* (<cond><set-op><cond>) */
        flag=1;
        cond->left=(ThreeChildren *)calloc(1,sizeof(ThreeChildren)); cond->degreeOfLeftChild=3;
        cond->right=(ThreeChildren *)calloc(1,sizeof(ThreeChildren)); cond->degreeOfRightChild=3;
        int randIndex=randInt(0,files->sizeSetOp-1);
        int size=strlen(files->setOp[randIndex]);
        cond->center=(OneChild *)calloc(1,sizeof(OneChild));
        ((OneChild *)(cond->center))->center=(NoChild *)calloc(1,sizeof(NoChild));
        ((NoChild *)(((OneChild *)(cond->center))->center))->data=(char *)calloc(size+1,sizeof(char));
        strcpy(((NoChild *)(((OneChild *)(cond->center))->center))->data,files->setOp[randIndex]);
        createForHaveThreeChildren((ThreeChildren *)(cond->left),randInt(1,2),randInt(3,5),randInt(3,5),files);
        createForHaveThreeChildren((ThreeChildren *)(cond->right),randInt(1,2),randInt(3,5),randInt(3,5),files);
    }
    else if(rand==2){ /* (<expr><rel-op><expr>) */
        int randIndex=randInt(0,files->sizeRelOp-1);
        int size=strlen(files->relOp[randIndex]);
        cond->center=(OneChild *)calloc(1,sizeof(OneChild));
        ((OneChild *)(cond->center))->center=(NoChild *)calloc(1,sizeof(NoChild));
        ((NoChild *)(((OneChild *)(cond->center))->center))->data=(char *)calloc(size+1,sizeof(char));
        strcpy(((NoChild *)(((OneChild *)(cond->center))->center))->data,files->relOp[randIndex]);
    }
    if(flag==0){
        /* for left child */
        if(randLeft==3){ /* (<expr><op><expr>) */
            cond->left=(ThreeChildren *)calloc(1,sizeof(ThreeChildren)); cond->degreeOfLeftChild=3;
            int randIndex=randInt(0,files->sizeOp-1);
            int size=strlen(files->op[randIndex]);
            ((ThreeChildren *)(cond->left))->center=(OneChild *)calloc(1,sizeof(OneChild));
            ((OneChild *)(((ThreeChildren *)(cond->left))->center))->center=(NoChild *)calloc(1,sizeof(NoChild));
            ((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->left))->center))->center))->data=(char *)calloc(size+1,sizeof(char));
            strcpy(((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->left))->center))->center))->data,files->op[randIndex]);
            createForHaveThreeChildren((ThreeChildren *)(cond->left),0,randInt(3,5),randInt(3,5),files);
        }
        else if(randLeft==4){ /* <pre-op>(<expr>) */
            cond->left=(TwoChildren *)calloc(1,sizeof(TwoChildren)); cond->degreeOfLeftChild=2;
            createForHaveTwoChildren((TwoChildren *)(cond->left),randInt(3,5),files);
        }
        else if(randLeft==5){ /* <var> */
            int randIndex=randInt(0,files->SizeVar-1);
            int size=strlen(files->var[randIndex]);
            cond->left=(OneChild *)calloc(1,sizeof(OneChild)); cond->degreeOfLeftChild=1;
            ((OneChild *)(cond->left))->center=(NoChild *)calloc(1,sizeof(NoChild));
            ((NoChild *)(((OneChild *)(cond->left))->center))->data=(char *)calloc(size+1,sizeof(char));
            strcpy(((NoChild *)(((OneChild *)(cond->left))->center))->data,files->var[randIndex]);
        }
        /* for right child */
        if(randRight==3){ /* (<expr><op><expr>) */
            cond->right=(ThreeChildren *)calloc(1,sizeof(ThreeChildren)); cond->degreeOfRightChild=3;
            int randIndex=randInt(0,files->sizeOp-1);
            int size=strlen(files->op[randIndex]);
            ((ThreeChildren *)(cond->right))->center=(OneChild *)calloc(1,sizeof(OneChild));
            ((OneChild *)(((ThreeChildren *)(cond->right))->center))->center=(NoChild *)calloc(1,sizeof(NoChild));
            ((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->right))->center))->center))->data=(char *)calloc(size+1,sizeof(char));
            strcpy(((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->right))->center))->center))->data,files->op[randIndex]);
            createForHaveThreeChildren((ThreeChildren *)(cond->right),0,randInt(3,5),randInt(3,5),files);
        }
        else if(randRight==4){ /* <pre-op>(<expr>) */
            cond->right=(TwoChildren *)calloc(1,sizeof(TwoChildren)); cond->degreeOfRightChild=2;
            createForHaveTwoChildren((TwoChildren *)(cond->right),randInt(3,5),files);
        }
        else if(randRight==5){ /* <var> */
            int randIndex=randInt(0,files->SizeVar-1);
            int size=strlen(files->var[randIndex]);
            cond->right=(OneChild *)calloc(1,sizeof(OneChild)); cond->degreeOfRightChild=1;
            ((OneChild *)(cond->right))->center=(NoChild *)calloc(1,sizeof(NoChild));
            ((NoChild *)(((OneChild *)(cond->right))->center))->data=(char *)calloc(size+1,sizeof(char));
            strcpy(((NoChild *)(((OneChild *)(cond->right))->center))->data,files->var[randIndex]);
        }
    }
}
void createForHaveTwoChildren(TwoChildren *cond,int rand,Files *files){
    int randIndex=randInt(0,files->sizePreOp-1);
    int size=strlen(files->preOp[randIndex]);
    cond->left=(OneChild *)calloc(1,sizeof(OneChild));
    ((OneChild *)(cond->left))->center=(NoChild *)calloc(1,sizeof(NoChild));
    ((NoChild *)(((OneChild *)(cond->left))->center))->data=(char *)calloc(size+1,sizeof(char));
    strcpy(((NoChild *)(((OneChild *)(cond->left))->center))->data,files->preOp[randIndex]);
    if(rand==3){ /* (<expr><op><expr>) */
        int randIndex=randInt(0,files->sizeOp-1);
        int size=strlen(files->op[randIndex]);
        cond->right=(ThreeChildren *)calloc(1,sizeof(ThreeChildren)); cond->degreeOfRightChild=3;
        ((ThreeChildren *)(cond->right))->center=(OneChild *)calloc(1,sizeof(OneChild));
        ((OneChild *)(((ThreeChildren *)(cond->right))->center))->center=(NoChild *)calloc(1,sizeof(NoChild));
        ((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->right))->center))->center))->data=(char *)calloc(1,sizeof(char));
        strcpy(((NoChild *)(((OneChild *)(((ThreeChildren *)(cond->right))->center))->center))->data,files->op[randIndex]);
        createForHaveThreeChildren((ThreeChildren *)(cond->right),0,randInt(3,5),randInt(3,5),files);
    }
    else if(rand==4){ /* <pre-op>(<expr>) */
        cond->right=(TwoChildren *)calloc(1,sizeof(TwoChildren)); cond->degreeOfRightChild=2;
        createForHaveTwoChildren((TwoChildren *)(cond->right),randInt(3,5),files);
    }
    else if(rand==5){ /* <var> */
        int randIndex=randInt(0,files->SizeVar-1);
        int size=strlen(files->var[randIndex]);
        cond->right=(OneChild *)calloc(1,sizeof(OneChild)); cond->degreeOfRightChild=1;
        ((OneChild *)(cond->right))->center=(NoChild *)calloc(1,sizeof(NoChild));
        ((NoChild *)(((OneChild *)(cond->right))->center))->data=(char *)calloc(size+1,sizeof(char));
        strcpy(((NoChild *)(((OneChild *)(cond->right))->center))->data,files->var[randIndex]);
    }
}
void traverseForHaveThreeChildren(ThreeChildren *cond){
    printf("(");
    if(cond->degreeOfLeftChild==3)
        traverseForHaveThreeChildren((ThreeChildren *)(cond->left));
    else if(cond->degreeOfLeftChild==2)
        traverseForHaveTwoChildren((TwoChildren *)(cond->left));
    else if(cond->degreeOfLeftChild==1)
        printf("(%s)",((NoChild *)(((OneChild *)(cond->left))->center))->data);
    printf("%s",((NoChild *)(((OneChild *)(cond->center))->center))->data);
    if(cond->degreeOfRightChild==3)
        traverseForHaveThreeChildren((ThreeChildren *)(cond->right));
    else if(cond->degreeOfRightChild==2)
        traverseForHaveTwoChildren((TwoChildren *)(cond->right));
    else if(cond->degreeOfRightChild==1)
        printf("(%s)",((NoChild *)(((OneChild *)(cond->right))->center))->data);
    printf(")");
}
void traverseForHaveTwoChildren(TwoChildren *cond){
    printf("(%s",((NoChild *)(((OneChild *)(cond->left))->center))->data);
    if(cond->degreeOfRightChild==3)
        traverseForHaveThreeChildren((ThreeChildren *)(cond->right));
    else if(cond->degreeOfRightChild==2)
        traverseForHaveTwoChildren((TwoChildren *)(cond->right));
    else if(cond->degreeOfRightChild==1)
        printf("(%s)",((NoChild *)(((OneChild *)(cond->right))->center))->data);
    printf(")");
}