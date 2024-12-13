#define IsWord 1
#define NotWord 0
#define validwords 70

extern struct trie Directory;


struct trie 
{
    struct trie * next[validwords];
    int word_status;
    int  id;
};