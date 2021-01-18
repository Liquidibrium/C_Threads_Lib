#include "linked_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
static void FreeString(void *elemAddr) {
  char *s = *(char **) elemAddr;
  free(s); 
}

void testMemo(){
  int i;
  const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
  const int kNumQuestionWords = 5; // sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
  LinkedQueue questionWords;
  LinkedQueueInit(&questionWords, sizeof(char *), FreeString);
  for (i = 0; i < kNumQuestionWords; i++) {
    char * questionWord = strdup(kQuestionWords[i]);
    LinkedQueuePushBack(&questionWords, &questionWord);
  }
  for (i = 0; i < kNumQuestionWords; i++) {
    char * res; 
    LinkedQueueGetFront(&questionWords, &res); 
   // printf("%s\n",res);
    assert(strcmp(kQuestionWords[i],res)==0);
  }
  LinkedQueueDispose(&questionWords);
  printf("\nSuccess Memo Test\n");
}


void simpleTest2(){ 
  LinkedQueue q;
  LinkedQueueInit(&q, sizeof(int), NULL);
  for(int i = 1 ; i<=1000 ; i++){
    LinkedQueuePushBack(&q, &i);
    assert(LinkedQueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 1000 ; i++){
    int res;
    LinkedQueuePeekFront(&q, &res); 
    int res2;
    LinkedQueueGetFront(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }
  LinkedQueueDispose(&q); 
  printf("\nSuccess Simple Test2\n");
}
void simpleTest1(){ 
  LinkedQueue q;
  LinkedQueueInit(&q, sizeof(int), NULL);
  int add100  = 100 ;
  int add103 = 103 ;
  LinkedQueuePushBack(&q, &add100);
  LinkedQueuePushBack(&q, &add103);
  assert(LinkedQueueSize(&q)==2);
  int res100;
  int res103;
  LinkedQueuePeekFront(&q, &res100); 
  LinkedQueueGetFront(&q,&res100);
  assert(res100==100);
  LinkedQueueGetFront(&q,&res103);
  assert(res103==103);
  LinkedQueueDispose(&q); 
  printf("\nSuccess Simple Test1\n");
}
int main(){
  simpleTest1();
  simpleTest2();
  testMemo();
  return 0;
}