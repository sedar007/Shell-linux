#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF_LEN 1024
#define MAX_ARGV_LEN 10

typedef
struct cmd //
{
    char *argv[MAX_ARGV_LEN]; //arguments de la commande
    char *eargv[MAX_ARGV_LEN]; //arguments d'environnement 
} cmd;

//char tab[10]; <=> char * tab; 
//char **argv:  argv[0]

char buf[BUF_LEN];

struct cmd *parsecmd(char *s);



//************************** l'implémentation de parsecmd() **************//

int my_strlen(const char *s)
{
    int n;
    for (n = 0; s[n]; n++)
        ;
    return n;
}

char *my_strchr(const char *s, char c)
{
    for (; *s; s++)
        if (*s == c)
            return (char *)s;
    return 0;
}

char WHITECHARS[] = "\t\n ";

int peek(char **ps, char *es, char *toks)
{
    // Move to non-space char, check if exists tokens specified in params.
    char *s;

    s = *ps;
    while (s < es && strchr(WHITECHARS, *s))
        s++;
    *ps = s;
    return *s && strchr(toks, *s);
}

int gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s = *ps;
    while (s < es && my_strchr(WHITECHARS, *s))
        s++;
    if (q)
        *q = s;

    int ret = *s;
    // now we have a non-whitespace char that s points to.
    switch (*s)
    {
    case 0:
        break;
    case '|':
    case '<':
    case '>':
        s++; // skip it.
        break;
    default:
        ret = 'a';
        // loop str stops when see whitespace.
        while (s < es && !my_strchr(WHITECHARS, *s))
            s++;
        break;
    }
    if (eq)
        *eq = s;
    // Make new ps points to non-whitespace char.
    while (s < es && my_strchr(WHITECHARS, *s))
        s++;
    *ps = s;

    return ret;
}

struct cmd *parsecmd(char *s)
{
    // 1. allocation mémoire de la commande (struct cmd *)
    struct cmd *ec;
    ec = malloc(sizeof(*ec));
    memset(ec, 0, sizeof(*ec));

    // 2. recupération de la commande en entrée depuis char *s
    char *q, *eq;
    char *es = s + my_strlen(s);
    int i = 0;
    while (!peek(&s, es, "|"))
    {
        if (gettoken(&s, es, &q, &eq) == '\0')
        {
            break;
        }
        ec->argv[i] = q;
        ec->eargv[i] = eq;
        i++;
    }

    for (int i = 0; ec->argv[i]; i++)
        *ec->eargv[i] = 0;
    return ec;
}

//************************** fin de l'implémentation de parsecmd() **************//



int main(void)
{
    while (1)
    {
        printf(1,"Entrer une commande > ");
        
        char *s =  gets(buf,BUF_LEN);
        cmd *cm = parsecmd(s); 
        

        if( (*s == '\n') ||  strcmp("quit",s) == 0 ){ 
            printf(1,"Fin \n");
            exit();
        }
               
        int pid = fork();

        if ( pid > 0){  
            pid = wait();
                            
        } 

        else if (pid == 0) {
            
                exec(cm->argv[0], cm->argv);
            
            
            printf(1,"\n");
            
            exit();
        } 
       
    }
}