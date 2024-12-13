#include "header.h"
#include "function_def.h"
#include "Path_ds.h"
#include "basic_header.h"

int getkey(char a)
{
    if (a == 'a')
    {
        return 0;
    }
    else if (a == 'b')
    {
        return 1;
    }
    else if (a == 'c')
    {
        return 2;
    }
    else if (a == 'd')
    {
        return 3;
    }
    else if (a == 'e')
    {
        return 4;
    }
    else if (a == 'f')
    {
        return 5;
    }
    else if (a == 'g')
    {
        return 6;
    }
    else if (a == 'h')
    {
        return 7;
    }
    else if (a == 'i')
    {
        return 8;
    }
    else if (a == 'j')
    {
        return 9;
    }
    else if (a == 'k')
    {
        return 10;
    }
    else if (a == 'l')
    {
        return 11;
    }
    else if (a == 'm')
    {
        return 12;
    }
    else if (a == 'n')
    {
        return 13;
    }
    else if (a == 'o')
    {
        return 14;
    }
    else if (a == 'p')
    {
        return 15;
    }
    else if (a == 'q')
    {
        return 16;
    }
    else if (a == 'r')
    {
        return 17;
    }
    else if (a == 's')
    {
        return 18;
    }
    else if (a == 't')
    {
        return 19;
    }
    else if (a == 'u')
    {
        return 20;
    }
    else if (a == 'v')
    {
        return 21;
    }
    else if (a == 'w')
    {
        return 22;
    }
    else if (a == 'x')
    {
        return 23;
    }
    else if (a == 'y')
    {
        return 24;
    }
    else if (a == 'z')
    {
        return 25;
    }
    else if (a == 'A')
    {
        return 26;
    }
    else if (a == 'B')
    {
        return 27;
    }
    else if (a == 'C')
    {
        return 28;
    }
    else if (a == 'D')
    {
        return 29;
    }
    else if (a == 'E')
    {
        return 30;
    }
    else if (a == 'F')
    {
        return 31;
    }
    else if (a == 'G')
    {
        return 32;
    }
    else if (a == 'H')
    {
        return 33;
    }
    else if (a == 'I')
    {
        return 34;
    }
    else if (a == 'J')
    {
        return 35;
    }
    else if (a == 'K')
    {
        return 36;
    }
    else if (a == 'L')
    {
        return 37;
    }
    else if (a == 'M')
    {
        return 38;
    }
    else if (a == 'N')
    {
        return 39;
    }
    else if (a == 'O')
    {
        return 40;
    }
    else if (a == 'P')
    {
        return 41;
    }
    else if (a == 'Q')
    {
        return 42;
    }
    else if (a == 'R')
    {
        return 43;
    }
    else if (a == 'S')
    {
        return 44;
    }
    else if (a == 'T')
    {
        return 45;
    }
    else if (a == 'U')
    {
        return 46;
    }
    else if (a == 'V')
    {
        return 47;
    }
    else if (a == 'W')
    {
        return 48;
    }
    else if (a == 'X')
    {
        return 49;
    }
    else if (a == 'Y')
    {
        return 50;
    }
    else if (a == 'Z')
    {
        return 51;
    }
    else if (a == '/')
        return 52;
    else if (a == '.')
        return 53;
    else if (a == '_')
        return 54;
    else if (a == '-')
        return 55;
    else if (a == '0')
        return 56;
    else if (a == '1')
        return 57;
    else if (a == '2')
        return 58;
    else if (a == '3')
        return 59;
    else if (a == '4')
        return 60;
    else if (a == '5')
        return 61;
    else if (a == '6')
        return 62;
    else if (a == '7')
        return 63;
    else if (a == '8')
        return 64;
    else if (a == '9')
        return 65;
    else if (a == '*')
        return 66;
    else if (a == ' ')
        return 67;
    else if (a == '+')
        return 68;

    else
    {
        return -1;
    }
}

void insert_rec(char *a, int idx, int id, struct trie *tree)
{

    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];
    if (temp == NULL) // if (cnt <= 0)
    {
        tree->next[cnt] = malloc(sizeof(struct trie));
        for (int i = 0; i < validwords; i++)
        {
            tree->next[cnt]->next[i] = NULL;
        }
    }
    if (a[idx + 1] == '\0' || a[idx + 1] == '\n')
    {
        tree->next[cnt]->word_status = IsWord;
        tree->next[cnt]->id = id;
        return;
    }
    insert_rec(a, idx + 1, id, tree->next[cnt]);
    return;
}

int find_rec_path(char a[256], int idx, struct trie *tree)
{
    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];

    if (tree->next[cnt] == NULL)
    {
        return -1;
    }
    if (a[idx + 1] == '\0' || a[idx + 1] == '\n')
    {
        if (temp->word_status == IsWord)
            return temp->id;
        else
            return -1;
    }
    return find_rec_path(a, idx + 1, temp);
}

int delete_rec_path(char a[256], int idx, struct trie *tree)
{
    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];

    if (tree->next[cnt] == NULL)
    {
        return -1;
    }
    if (a[idx + 1] == '\0' || a[idx + 1] == '\n')
    {
        if (temp->word_status == IsWord)
        {
            temp->word_status=0;
            return 0;
        }
        else
            return -1;
    }
    return find_rec_path(a, idx + 1, temp);
}

void Insert_Dir(char a[200][256], int path_num, int id)
{
    for (int i = 0; i < path_num; i++)
    {
        insert_rec(a[i], 0, id, &Directory);
    }
}

int Find_Path(char *a)
{
    int ans = find_rec_path(a, 0, &Directory);
    return ans;
}

void intialise_Directory(void)
{
    for (int i = 0; i < validwords; i++)
        Directory.next[i] = NULL;
    return;
}