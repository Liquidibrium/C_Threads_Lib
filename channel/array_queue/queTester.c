#include "array_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
static void FreeString(void *elemAddr) {
  char *s = *(char **) elemAddr;
  free(s); 
}
static void PrintString(void *elemAddr) {
  char *word = *(char **)elemAddr;
  printf("\t%s\n", word);
}

void testMemo1(){
  int i;
  const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
  const int kNumQuestionWords = 5; // sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
  ArrayQueue questionWords;
  ArrayQueueInit(&questionWords, 10, sizeof(char *), FreeString);
  for (i = 0; i < kNumQuestionWords; i++) { //     0 1 2 3 4 f-0 l-4 
    char * questionWord = strdup(kQuestionWords[i]);
    ArrayQueuePushBack(&questionWords, &questionWord);
  }
  for (i = 0; i < kNumQuestionWords-1; i++) { // 0 1 2 3 4  f-4 l-4 
    char * res; 
    ArrayQueueGetFront(&questionWords, &res); 
    //printf("%s\n",res);
    assert(strcmp(kQuestionWords[i],res)==0);
    free(res);
  }
  ArrayQueueDispose(&questionWords);
  printf("\nSuccess Memo Test1\n");
}

void testMemo2(){
  int i;
  const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
  const int kNumQuestionWords = 5; // sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
  ArrayQueue questionWords;
  ArrayQueueInit(&questionWords, 4, sizeof(char *), FreeString);
  for (i = 0; i < 4; i++) {    ///          0 1 2 3  f-0 l-3   
      char * questionWord = strdup(kQuestionWords[i]);
    ArrayQueuePushBack(&questionWords, &questionWord);
  }
  char * res; 
    ArrayQueueGetFront(&questionWords, &res);  // f-1 l-3 
    assert(strcmp(kQuestionWords[0],res)==0);
    free(res);
    char * questionWord = strdup(kQuestionWords[4]);
    ArrayQueuePushBack(&questionWords, &questionWord);  //f-1 l-0 

    assert(-1 == ArrayQueuePushBack(&questionWords, &questionWord));  //f-1 l-0 
  
  ArrayQueueDispose(&questionWords);
  printf("\nSuccess Memo Test2\n");
}

void simpleTest2(){ 
  ArrayQueue q;
  ArrayQueueInit(&q, 1000, sizeof(int), NULL);
  for(int i = 1 ; i<=1000 ; i++){
    ArrayQueuePushBack(&q, &i);
    assert(ArrayQueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 1000 ; i++){
    int res;
    ArrayQueuePeekFront(&q, &res); 
    int res2;
    ArrayQueueGetFront(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }

  for(int i = 1 ; i<=1000 ; i++){
    ArrayQueuePushBack(&q, &i);
    assert(ArrayQueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 1000 ; i++){
    int res;
    ArrayQueuePeekFront(&q, &res); 
    int res2;
    ArrayQueueGetFront(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }
  for(int i = 1 ; i<=500 ; i++){
    ArrayQueuePushBack(&q, &i);
    assert(ArrayQueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 500 ; i++){
    int res;
    ArrayQueuePeekFront(&q, &res); 
    int res2;
    ArrayQueueGetFront(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }
  for(int i = 1 ; i<=500 ; i++){
    ArrayQueuePushBack(&q, &i);
    assert(ArrayQueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 500 ; i++){
    int res;
    ArrayQueuePeekFront(&q, &res); 
    int res2;
    ArrayQueueGetFront(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }
  ArrayQueueDispose(&q); 
  printf("\nSuccess Simple Test2\n");
}
void simpleTest1(){ 
  ArrayQueue q;
  ArrayQueueInit(&q,4, sizeof(int), NULL);
  int add100  = 100 ;
  int add103 = 103 ;
  ArrayQueuePushBack(&q, &add100);
  ArrayQueuePushBack(&q, &add103);
  assert(ArrayQueueSize(&q)==2);
  int res100;
  int res103;
  ArrayQueuePeekFront(&q, &res100); 
  ArrayQueueGetFront(&q,&res100);
  assert(res100==100);
  ArrayQueueGetFront(&q,&res103);
  assert(res103==103);
  ArrayQueueDispose(&q); 
  printf("\nSuccess Simple Test1\n");
}
int main(){
  simpleTest1();
  simpleTest2();
  testMemo1();
  testMemo2();
  return 0;
}