#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#define __NR_get_address 436

int global_in = 10; //data segment
int global_nin; //BSS segment
int *heap; //heap segment
void *libraries; //libraries
void text() //text 
{
    printf( "hello\n");
}

long get_address( void *addr )
{
    return syscall( 700, addr );
}

void *child( void *arg )
{
    int local = 123;
    int *id = arg;


/*    
    printf( "This is thread %d:\n", *id );
    printf( "[global_in] vaddr: %p, paddr:  --data\n", &global_in );
    printf( "[global_nin] vaddr: %p, paddr:  --BSS\n", &global_nin );
    printf( "[heap] vaddr: %p, paddr:  --heap\n", &heap );
    printf( "[libraries] vaddr: %p, paddr:  --libraries\n", libraries );
    printf( "[text] vaddr: %p, paddr:  --text\n", &text );
    printf( "[local] vaddr: %p, paddr:  --stack\n", &local );
    printf( "\n" );
*/

    sleep(*id);

    printf( "This is thread %d:\n", *id );
    printf( "[global_in] vaddr: %p, paddr: %p --data\n", &global_in, (void *)get_address( &global_in ) );
    printf( "[global_nin] vaddr: %p, paddr: %p --BSS\n", &global_nin, (void *)get_address( &global_nin ) );
    printf( "[heap] vaddr: %p, paddr: %p --heap\n", &heap, (void *)get_address( &heap ) );
    printf( "[libraries] vaddr: %p, paddr: %p  --libraries\n", &libraries, (void *)get_address( libraries ) );
    printf( "[text] vaddr: %p, paddr: %p --text\n", &text, (void *)get_address( &text ) );
    printf( "[local] vaddr: %p, paddr: %p --stack\n", &local, (void *)get_address( &local ) );
    printf( "\n" );

    return NULL;

}

int main()
{

    heap = malloc( sizeof(int) * 10 );  
    libraries = &printf;
    
    pthread_t id1, id2, id3;
    int id[3] = { 0, 1, 2 };
    int a=1,b=2;
    //syscall( 701, 143 );
    //int r=syscall(702, 12,13 );
    //printf( "this is b %d\n", r );
    //printf("%p, %p\n%p, %p\n",&a, get_address( &a ),&global_in, (void *)get_address( &global_in ));
    //printf("%p, %p\n",&a, (void *)get_address( &a ));
  
    pthread_create( &id1, NULL, child, &id[0] );
    pthread_create( &id2, NULL, child, &id[1] );
    pthread_create( &id3, NULL, child, &id[2] );

    pthread_join( id1, NULL );
    pthread_join( id2, NULL );
    pthread_join( id3, NULL );
    sleep(1);

    printf( "---program ends---\n" );
    
    return 0;

}
