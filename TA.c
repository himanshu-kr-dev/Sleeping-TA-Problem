//Author :: Himanshu Kumar (11802503)

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>
//for cross platform functioning
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
//
#define pf printf
#define sf scanf
#define ll long 

typedef sem_t st;
typedef pthread_t ptht;
typedef pthread_mutex_t pthmt;

ptht *Students;  
ptht ta; 

st chair[3];
st stu;
st sleepingTA;

ll cntChairs = 0;
pthmt OccChairs;
ll chIndex = 0;
ll spentTime;

void *testTA();
void *testSTU();

void simulate() {
    ll n = 5;
    /*pf("Enter the numer of students ");
    sf("%d",n);*/

    srand(time(NULL));
    sem_init(&sleepingTA, 0, 0);
    sem_init(&stu, 0, 0);
    ll i = 0;
    for (i = 0; i < 3; ++i)
        sem_init(&chair[i], 0, 0);

    pthread_mutex_init(&OccChairs, NULL);

    Students = (pthread_t *) malloc(sizeof(pthread_t) * n);

    pthread_create(&ta, NULL, testTA, (void *) (long) i);

    for (i = 0; i < n; i++) {
        pthread_create(&Students[i], NULL, testSTU, (void *) (long) i);
    }
    ll test[600];
    for (int i = 0; i < 600; ++i) test[i] = 1;
    pthread_join(ta, NULL);
    for (i = 0; i < n; i++)
        pthread_join(Students[i], NULL);
    free(Students);

}
int main()
{
    simulate();
}
void *testTA() {
    while (1) {
        sem_wait(&sleepingTA);
        pf("Student enters the Office..\n");
        pf("TA wakes up..\n");
        while (1) {
            pthread_mutex_lock(&OccChairs);
            if (cntChairs == 0) {
                pthread_mutex_unlock(&OccChairs);
                break;

            }

            sem_post(&chair[chIndex]);
            cntChairs--;
            pf("Student  left his/her chair in waiting room n goes to ta. Remaining Chairs %ld..\n", 3 - cntChairs);
            chIndex = (chIndex + 1) % 3;
            pthread_mutex_unlock(&OccChairs);
            pf("TA is currently helping the student..\n");
            ll test[600];
            for (int i = 0; i < 600; ++i) test[i] = 1;
            sleep(5);
            sem_post(&stu);
            usleep(1000);


        }

    }

}


void *testSTU(void *thID) {
    spentTime = 0;
    while (1) {

        pf("Student number :%ld is doing work..\n", (long) thID);

        spentTime = rand() % 10 + 1;
        sleep(spentTime);
        pf("Student %ld needs help..\n", (long) thID);
        pthread_mutex_lock(&OccChairs);
        ll cnt = cntChairs;

        pthread_mutex_unlock(&OccChairs);
        if (cnt < 3) {
            if (cnt == 0)
                sem_post(&sleepingTA);
            else
                pf("Student %ld sat on a chair waiting for the TA to finish.. \n", (long) thID);

            pthread_mutex_lock(&OccChairs);

            int ind = (chIndex + cntChairs) % 3;
            cntChairs++;
            pf("Student settled down on chair, now chairs remaining are %ld..\n", 3 - cntChairs);
            pthread_mutex_unlock(&OccChairs);

            sem_wait(&chair[ind]);
            pf("Teahing Assistant is helping student number :: %ld ..\n", (long) thID);
            sem_wait(&stu);
            pf("Student :: %ld walks out of TA room..\n", (long) thID);
            ll test[600] = {0};
            for (int i = 0; i < 600; ++i) test[i] = 1;

        } else {
            pf("Student :: %ld will return after some time..\n", (long) thID);
            ll test[600] = {0};
            for (int i = 0; i < 600; ++i) test[i] = 1;
        }


    }

}














