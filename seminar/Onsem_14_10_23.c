#include <stdio.h>
#include <stdlib.h>

struct Tree
{
    int n;
    struct Tree* L;
    struct Tree* R;
};

void deleteT(struct Tree* T)
{
    if(T->L != NULL)
    {
        deleteT(T->L);
    }
    if(T->R != NULL)
    {
        deleteT(T->R);
    }
    free(T);
    return;
}

struct Tree* read_Tree()
{
    struct Tree* T1 = NULL;
    struct Tree* T2 = NULL;
    int c;
    while(scanf("%d", &c) != EOF)
    {
        struct Tree* Tb = calloc(1, sizeof(struct Tree));
        Tb->n = c;
        Tb->R = NULL;
        Tb->L = NULL;
        if(T1 == NULL)
        {
            T1 = Tb;
        }
        else
        {
            T2 = T1;
            while(1)
            {
                if((c < T2->n ) && (T2->L != NULL))
                {
                    T2 = T2->L;
                }
                else if((c > T2->n ) && (T2->R != NULL))
                {
                    T2 = T2->R;
                }
                else
                {
                    break;
                }
            }
            if(c < T2->n)
                {
                    T2->L = Tb;
                }
                else if(c > T2->n)
                {
                    T2->R = Tb;
                }
                else
                {
                    free(Tb);
                }
        }
    }

    return T1;
}

int min(int a, int b)
{

    if(a >= b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

void print_Tree(struct Tree* p, int level)
{
    if(p != NULL)
    {
        print_Tree(p->L,level + 1);
        for(int i = 0;i< level;i++)
        printf("   ");
        printf("%d\n", p->n);
        print_Tree(p->R,level + 1);
    }
}

int find_inTree(struct Tree* T, int F, int level)
{
    int a = -1;
    int b = -1;
    if(F == T->n)
    {
        return level;
    }
    if((F < T->n ) && (T->L != NULL))
    {
        a = find_inTree(T->L, F, level + 1);
    }
    else if((F > T->n ) && (T->R != NULL))
    {
        b = find_inTree(T->R, F, level + 1);
    }
    else
    {
        return -1;
    }
    if(a == -1)
    {
        return b;
    }
    if(b == -1)
    {
        return a;
    }
    return min(a, b);
}




struct Tree* DeleteNode(struct Tree* node, int val) // из инета, переписанна с с++ под си
{
    if(node == NULL)
    {
        return node;
    }
    if(val == node->n)
    {
        struct Tree* tmp;
        if(node->R == NULL) // правого поддерева нету, удаление заменит удаляемый элемент на его левое поддерево
        {
            tmp = node->L;
        }
        else
        {
            struct Tree* ptr = node->R;
            if(ptr->L == NULL) // -||-
            {
                ptr->L = node->L;
                tmp = ptr;
            }
            else // всё не оч как то, прийдется морочиться
            {
                struct Tree* pmin = ptr->L;
                while(pmin->L != NULL) // перестраиваем дерево чтобы можно было удалить
                {
                    ptr = pmin;
                    pmin = ptr->L;
                }
                ptr->L = pmin->R;
                pmin->L = node->L;
                pmin->R = node->R;
                tmp = pmin;
            }
        }
        free(node);
        return tmp;
    }
    else if(val < node->n) // рекурсивный поиск удаляемого элемента
    {
        node->L = DeleteNode(node->L, val);
    }
    else
    {
        node->R = DeleteNode(node->R, val);
    }
    return node;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b; 
}

int height(struct Tree* T) // рекурсивная функция поиска высоты
{
    if(T == NULL)
    {
        return 0;
    }
    else
    {
        return (1 + max(height(T->L), height(T->R)));
    }
    
}

struct Tree* rotateright(struct Tree* p) // правый поворот вокруг p
{
	struct Tree* q = p->L;
	p->L = q->R;
	q->R = p;
	return q;
}

struct Tree* rotateleft(struct Tree* q) // левый поворот вокруг q
{
	struct Tree* p = q->R;
	q->R = p->L;
	p->L = q;
	return p;
}

int bfactor(struct Tree* p) // разность высоты поддеревьев
{
	return height(p->R)-height(p->L);
}

struct Tree* balance(struct Tree* p) // балансировка узла p взята из инета, переписанна под си
{
    if(p->L != NULL)
    {
        p->L = balance(p->L); // изначально функция работала только при разности высот 2, но добавив рекурсию и ">=" начала работать, разве это не чудесно?)
    }
    if(p->R != NULL)
    {
        p->R = balance(p->R);
    }
	if( bfactor(p)>=2 )
	{
		if( bfactor(p->R) < 0 )
			p->R = rotateright(p->R);
        p = rotateleft(p);
		return p;
	}
	if( bfactor(p)<=-2 )
	{
		if( bfactor(p->L) > 0  )
			p->L = rotateleft(p->L);
        p = rotateright(p);
		return p;
	}
    
	return p; // балансировка не нужна
}
char balanced(struct Tree* p) // проверка дерева на баланс
{
    if (p == NULL)
        return 0;
    int fl = bfactor(p); 
    //printf("\n%d\n", fl);
    if((fl >= 2) || (fl <= -2))
        return 1;
    return balanced(p->L) || balanced(p->R);
}

struct Tree* T_balance(struct Tree* p) // иначе не работала(
{
    while(balanced(p) == 1)
    {
        p = balance(p);
        printf("\n----------------\n");
        print_Tree(p, 0);
        printf("----------------\n");
    }

    return p;
}


int main(void)
{
    // 1-5 пункты сданы в классе
    struct Tree* T1 = NULL; // начало списка
    printf("Введите дерево:\n");
    T1 = read_Tree();
    printf("\n----------------\n");
    print_Tree(T1, 0);
    printf("----------------\n");
    clearerr(stdin);
    printf("Введите эелемент на удаление:\n");
    int c;
    scanf("%d", &c);
    printf("Длина пути = %d\n", find_inTree(T1, c, 0));
    printf("----------------\n");
    T1 = DeleteNode(T1, c);
    print_Tree(T1, 0);
    printf("----------------\n");
    T1 = T_balance(T1);
    //T1 = balance(T1);
    print_Tree(T1, 0);
    printf("----------------\n");
    //printf("%d\n", height(T1));
    deleteT(T1);
    return 0;
}