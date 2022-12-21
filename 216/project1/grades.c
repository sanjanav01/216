#include <math.h>
#include <stdio.h>
#define MAXIMUM_ASSIGNMENTS 50

/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: September 26, 2022
*/

void drop_scores(int info[], int na, int drop_count);
double numeric_scores(int info[], int na, int penalty, int drop_count);

/* Drops a given number of assignments with the lowest scores in a given
collection*/
void drop_scores(int info[], int na, int drop_count) {
    int min, score, counter = 0, index = 0, i;

    while (counter < drop_count) {
        min = info[1] * info[2];
        for (i = 0; i < (na * 4); i += 4) {
            /*score = points * assignment_weight*/
            score = info[i + 1] * info[i + 2];
            if (score != 0) {
                /*if the score of an assignment is already the minimum, it
                updates the minimum value only if it is smaller than the
                previously noted assignment*/
                if (score < min || (score == min && i < index)) {
                    index = i;
                    min = score;
                }
            }
        }
        /*sets the weight of the lowest score assignment to 0*/
        info[index + 2] = 0;
        counter++;
    }
}

/* Calculates the total score of all combined assignments taking into
account late penalties and dropped assignments.
Returns score as a double */
double numeric_scores(int info[], int na, int penalty, int drop_count) {
    int i, late_assignment;
    double weight_sum, score;

    if (drop_count > 0) {
        drop_scores(info, na, drop_count);
    }

    for (i = 0; i < (na * 4); i += 4) {
        weight_sum += info[i + 2];
    }

    for (i = 0; i < (na * 4); i += 4) {
        /* calculates the new score with late day penalties*/
        late_assignment = info[i + 1] - (info[i + 3] * penalty);

        if (late_assignment > 0) {
            score += late_assignment * (info[i + 2] / weight_sum);
        }
    }
    return score;
}




/* Recevies user input and returns the following key data points:
Numeric Score, Points Penaly per Day Late, Number of Assignments Dropped,
Values Provided for Each Assignment. The statistics are calculated based on
request, and in addition the method returns the mean and standard deviation. */
int main() {
    int i = 0, penalty, drop_count, na, info[MAXIMUM_ASSIGNMENTS * 4],
        counter = 1, copy_info[MAXIMUM_ASSIGNMENTS * 4];
    double weight_sum = 0.0, score, sum, mean, sd;
    char stats;

    /* Penalty, Drop, Stats Input*/
    scanf(" %d %d %c", &penalty, &drop_count, &stats);

    /*Number of Assignments*/
    scanf(" %d", &na);

    /*Values Provided for Each Assignment*/
    while (i < (na * 4)) {
        scanf(" %d, %d, %d, %d", &info[i], &info[i + 1], &info[i + 2],
              &info[i + 3]);
        i += 4;
    }

    for (i = 0; i < (na * 4); i+=4){
        weight_sum += info[i + 2];
    }

    /*returns 0 if the sum of the weights is greater than 100*/
    if (weight_sum > 100.0) {
        printf("ERROR: Invalid values provided\n");
        return 0;
    }

    for (i = 0; i < na * 4; i++) {
        copy_info[i] = info[i];
    }

    if (drop_count >= 0 && drop_count <= na - 1) {
        if (penalty < 0) {
            penalty = 0;
        }
        score = numeric_scores(copy_info, na, penalty, drop_count);
    } else {
        score = 0;
    }
    printf("Numeric Score: %5.4f\n", score);
    printf("Points Penalty Per Day Late: %d\n", penalty);
    printf("Number of Assignments Dropped: %d\n", drop_count);

    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");

    /* prints the assignments in order of assignment number*/
    while (counter <= na) {

        for (i = 0; i < (na * 4); i += 4) {
            if (info[i] == counter) {
                printf("%d, %d, %d, %d\n", info[i], info[i + 1], info[i + 2],
                       info[i + 3]);
            }
        }
        counter = counter + 1;
    }

    if (stats == 'Y' || stats == 'y') {

        for (i = 0; i < (na * 4); i += 4) {
            score = info[i + 1] - (info[i + 3] * penalty);
            if (score >= 0) {
                sum += score;
            }
        }
        mean = sum / (float)na;
        sum = 0;

        for (i = 0; i < (na * 4); i += 4) {
            score = info[i + 1] - (info[i + 3] * penalty);
            if (score < 0) {
                score = 0;
            }
            sum += pow((score - mean), 2);
        }
        sd = sqrt(sum / (float)na);

        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, sd);
    }

    return 0;
}
