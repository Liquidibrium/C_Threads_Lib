#include "queue.h"
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

void testMemo(){
  int i;
  const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
  const int kNumQuestionWords = 5; // sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
  Queue questionWords;
  QueueInit(&questionWords, sizeof(char *), FreeString);
  for (i = 0; i < kNumQuestionWords; i++) {
    char * questionWord = strdup(kQuestionWords[i]);
    Enqueue(&questionWords, &questionWord);
  }
  for (i = 0; i < kNumQuestionWords; i++) {
    char * res ; 
    Dequeue(&questionWords, &res); 
   // printf("%s\n",res);
    assert(strcmp(kQuestionWords[i],res)==0);
  }
  QueueDispose(&questionWords);
  printf("\nSuccess Memo Test\n");
}


void simpleTest2(){ 
  Queue q;
  QueueInit(&q, sizeof(int), NULL);
  for(int i = 1 ; i<=1000 ; i++){
    Enqueue(&q, &i);
    assert(QueueSize(&q)==i);
  }
  for(int i = 1 ; i <= 1000 ; i++){
    int res;
    QueueFront(&q, &res); 
    int res2;
    Dequeue(&q,&res2);
     assert(res==res2);
     assert(res==i);
  }
  QueueDispose(&q); 
  printf("\nSuccess Simple Test2\n");
}
void simpleTest1(){ 
  Queue q;
  QueueInit(&q, sizeof(int), NULL);
  int add100  = 100 ;
  int add103 = 103 ;
  Enqueue(&q, &add100);
  Enqueue(&q, &add103);
  assert(QueueSize(&q)==2);
  int res100;
  int res103;
  QueueFront(&q, &res100); 
  Dequeue(&q,&res100);
  assert(res100==100);
  Dequeue(&q,&res103);
  assert(res103==103);
  QueueDispose(&q); 
  printf("\nSuccess Simple Test1\n");
}
int main(){
  simpleTest1();
  simpleTest2();
  testMemo();
  return 0;
}