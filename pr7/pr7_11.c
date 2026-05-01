#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_JOBS 5
#define TOTAL_STEPS 5

typedef enum { READY, RUNNING, PAUSED, CANCELLED, COMPLETED } JobStatus;

typedef struct {
    int id;
    int progress;
    JobStatus status;
} Job;

void print_status(Job *jobs) {
    printf("\rQueue Status: ");
    for (int i = 0; i < MAX_JOBS; i++) {
        char s;
        if (jobs[i].status == COMPLETED) s = 'V';
        else if (jobs[i].status == PAUSED) s = 'P';
        else if (jobs[i].status == CANCELLED) s = 'X';
        else if (jobs[i].status == RUNNING) s = 'R';
        else s = '.';
        printf("[%d:%c] ", jobs[i].id, s);
    }
    fflush(stdout);
}

int main() {
    Job jobs[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs[i].id = i + 1;
        jobs[i].progress = 0;
        jobs[i].status = READY;
    }

    int active_jobs = MAX_JOBS;
    int cycle = 0;

    while (active_jobs > 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (jobs[i].status == READY) jobs[i].status = RUNNING;

            if (jobs[i].status == RUNNING) {
                jobs[i].progress++;
                if (jobs[i].progress >= TOTAL_STEPS) {
                    jobs[i].status = COMPLETED;
                    active_jobs--;
                }
            }
        }

        print_status(jobs);

        if (cycle == 1) {
            printf("\n[Event] Job 2 PAUSED.\n");
            jobs[1].status = PAUSED;
        }
        if (cycle == 2) {
            printf("\n[Event] Job 3 CANCELLED.\n");
            jobs[2].status = CANCELLED;
            active_jobs--;
        }
        if (cycle == 4) {
            printf("\n[Event] Job 2 RESUMED.\n");
            jobs[1].status = RUNNING;
        }

        cycle++;
        for(long long j = 0; j < 500000000; j++); 
    }

    printf("\n\nAll jobs processed. Execution finished.\n");
    return 0;
}