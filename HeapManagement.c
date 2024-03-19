#include<stdio.h>
#include<stdlib.h>
#define HEAP_SIZE 2000   //This is our heap block on which we will be managing malloc and free
typedef struct meta_data_tag 
{
    size_t size;                    //size of the block
    struct meta_data_tag *next;     //pointer to next free block
}meta_data;
meta_data* free_list;               //creating a free_list linked list globally
void* initialize()
{
    meta_data* heap;                //This is whole heap block
    heap=malloc(HEAP_SIZE);         //malloc returns a void*
    heap->size=HEAP_SIZE-(sizeof(meta_data));   //The size of available block is HEAP_SIZE-metadata size
    heap->next=NULL;
    free_list=heap;                 //at first whole block is free
    return heap;
}
void split(meta_data* prev,meta_data* ptr,size_t req_size)    //Splitting the block and allocating only needed size
{
     meta_data *new=(meta_data *)((void *)ptr+sizeof(meta_data)+req_size);
     new->size=(ptr->size)-req_size-sizeof(meta_data);
     if(prev!=NULL)
     {
          new->next=prev->next;
          prev->next=new;        //new is added into free_list
     }
     else
     {
          new->next=free_list->next;
          free_list=new;          //if the block is allocated at the start of heap
     }
     //new->next=prev->next;
     ptr->size=req_size;         //as we are splitting the block size is req size only
}
       
void* Malloc(size_t req_size)
{
    meta_data *ptr=free_list;
    meta_data *prev,*result;
    prev=NULL;
    if(ptr==NULL)
    {
          printf("Memory is full\n");
          result=NULL;
    }
    else{
    while(ptr && (ptr->size)<req_size)  //Using First-fit
    {
          prev=ptr;
          ptr=ptr->next;
    }
    if(ptr==NULL)     //If it goes through all the available blocks and become null
    {
          printf("Memory is not full but no sufficient contiguous block of the size %ld\n",req_size);
          result=NULL;
    }
    else
    {
    if(((ptr->size)-req_size)>sizeof(meta_data))  //if we have have enough space to fit meta_data and some more then split
    {
          split(prev,ptr,req_size);
          printf("Fitting block is allocated with a split\n");
    }
    else
    {                                                //if the exact before block is not free
          if(prev!=NULL)                        
          prev->next=ptr->next;
          else
          free_list=free_list->next;
    }
    result=(void *)((char *)ptr + sizeof(meta_data));   //malloc must return the allocated space not the meta_data of block
    printf("The space is successfully allocated\n");
    }
    }
    return result;
}
void merge(meta_data *ptr1,meta_data *ptr2)
{
     ptr1->size+=(ptr2->size)+sizeof(meta_data);   //ptr1 is previous to ptr2 and both are free then merge
}
void Free(void *ptr) {
    if(ptr!=NULL)                        //if we try to free null
    {
    meta_data *nptr = ptr;               //ptr points to space it should point to meta_data to carry operations
    --nptr;
    meta_data *qptr = free_list;
    meta_data *prev = NULL;

    if (qptr != NULL) {                  //i.e, if the free_list is not empty
        while ((qptr) && (qptr) < nptr) {  
            prev = qptr;
            qptr = qptr->next;
        }
        // Merge with the next block if it's free
        if ((meta_data *)((char *)nptr + sizeof(meta_data) + nptr->size) == qptr) {
            merge(nptr,qptr);
            nptr->next = (qptr->next);
            //Merge with the previous block if it is free
            if (prev != NULL && (meta_data *)((char *)prev + sizeof(meta_data) + prev->size) == nptr) {
            merge(prev, nptr);
            prev->next=nptr->next;
           }
           else if(prev!=NULL)  //if it is not free
           {
               prev->next=nptr;
           } 
           else                   //if prev is null
           {
                free_list=nptr;
           }
        }
        else if (prev != NULL && (meta_data *)((char *)prev + sizeof(meta_data) + prev->size) == nptr) {
            merge(prev, nptr);             //next block is not free but previous block is free
            prev->next=qptr;
        } 
        else                                //if both are not free
        {
            nptr->next=qptr;
            if(prev!=NULL)
            {
                prev->next=nptr;
            }
            else
            {
            free_list=nptr;
            }
        }
     }
     else {
        free_list = nptr;
        free_list->next = NULL;
    }

    printf("The ptr is successfully freed\n");
   }
   else
   {
     printf("The given space is not allocated\n");
   }	
}
void Displayfree()
{
     meta_data *ptr=free_list;
     printf("The available free blocks are:\n");
     printf("ADDRESS\t\tSIZE\n");
     while(ptr)
     {
           printf("%p\t\t%ld\n",ptr,ptr->size);
           ptr=ptr->next;
     }
}
int main()
{
      meta_data *heap;
      heap=initialize();
      int* a=(int *)Malloc(10*sizeof(int));
      char* b=(char *)Malloc(5*sizeof(char));
      float* c=(float *)Malloc(8*sizeof(float));
      char* d=(char *)Malloc(5*sizeof(char));
      //Free(c);
      /*int* a=(int *)Malloc(486*sizeof(int));
      int* b=(int *)Malloc(sizeof(int));
      int* c=(int *)Malloc(sizeof(int));*/
      //printf("%ld\n",sizeof(meta_data));
      Free(a);
      Free(c);
      Displayfree();
      free(heap);
      return 0;
}   






