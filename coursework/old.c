//20124921 shytl1 Ting Lou
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//define the width and height of the map
#define WIDTH 80
#define HEIGHT 40

typedef struct planner
{
    char name[16];
    double latitude;
    double longitude;
    struct planner *next;
} Planner;
Planner *loadFile(Planner *p, Planner *head);
void showTarget(Planner *head);
int checkWords(FILE *fp);
void freeAndQuit(Planner *p);
void searchTarget(Planner *head);
void plan(Planner *head);
double getDistance(double x1, double x2, double y1, double y2);
Planner *destroy(Planner *head, Planner *oldp);
void freeAll(Planner *head);
int checkWordLength(char str[], int oldflag);
double inputADouble();
int menuInput();
int main()
{
    Planner *head, *p;
    head = (Planner *)malloc(sizeof(Planner));
    head->next = NULL;
    p = head;
    int input;
    printf("1) Load a target file\n2) Show current targets\n3) Search a target\n4) Plan an airstrike\n5) Execute an airstrike\n6) Quit\n");

    while (1)
    {

        printf("Option: ");

        input = menuInput();
        if (input == -1)
        {
            printf("Unknown option.\n");
            continue;
        }
        else
        {
            //match
            if (input == 1)
            {
                p = loadFile(p, head);
            }
            else if (input == 2)
            {
                showTarget(head);
            }
            else if (input == 3)
            {
                searchTarget(head);
            }
            else if (input == 4)
            {
                plan(head);
            }
            else if (input == 5)
            {
                p = destroy(head, p);
            }
            else if (input == 6)
            {
                freeAll(head);
                return 0;
            }
        }
    }
}
//read the file and recognize invalid file
Planner *loadFile(Planner *p, Planner *head)
{
    //flag used to represent validation of the file.flag==1,valid;flag==0,invalid
    //valid used to represent whether the the word inside the file can be divided by 3
    //tooClose represent whether the distance between new point and previous pointis less than 0.1
    int i = 0, f, flag = 1, dot = 0, valid, tooClose = 0;
    FILE *fp;
    char fileName[100];
    char tempName[16], tempLatitude[16], tempLongitude[16];
    double latitude, longitude, distance;
    // int targetted = 0;
    Planner *pt, *pp, *p1;

    pp = p;

    printf("Enter a target file: ");
    scanf("%s", fileName);
    // printf("The file name is %s\n", fileName);
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("Invalid file.\n");
        return p;
    }
    //check the words contained in file can be directly divided by 3 without any reminder
    valid = checkWords(fp);

    if (valid == 0)
    {
        //invalid
        printf("Invalid file.\n");
        fclose(fp);
        return p;
    }
    //get back to the begignning of the file
    rewind(fp);

    while (1)
    {
        //Name
        f = fscanf(fp, "%s", tempName);

        if (f != 1)
        {
            //end of file
            break;
        }
        else
        {
            flag = checkWordLength(tempName, flag);
            while (i < strlen(tempName))
            {
                if (((tempName[i] >= '0') && (tempName[i] <= '9')) || ((tempName[i] >= 'a') && (tempName[i] <= 'z')) || ((tempName[i] >= 'A') && (tempName[i] <= 'Z')))
                {

                    ;
                }
                else
                {
                    //wrong format
                    flag = 0;
                }
                i++;
            }
        }
        //reset variable
        i = 0;
        dot = 0;

        //latitude
        f = fscanf(fp, "%s", tempLatitude);
        if (f != 1)
        {
            //end of file
            break;
        }
        else
        {
            flag = checkWordLength(tempLatitude, flag);
            while (i < strlen(tempLatitude))
            {
                //check whether the latitude has more than 1 dot
                if (tempLatitude[i] == '.')
                {
                    dot++;
                }

                if (dot > 1)
                {
                    //wrong format
                    flag = 0;
                }

                if (((tempLatitude[i] >= '0') && (tempLatitude[i] <= '9')) || (tempLatitude[i] == '.'))
                {
                    ;
                }
                else
                {
                    //wrong format
                    flag = 0;
                }
                i++;
            }
        }

        //longitude
        f = fscanf(fp, "%s", tempLongitude);
        dot = 0;
        i = 0;
        if (f != 1)
        {
            //end of file
            break;
        }
        else
        {
            flag = checkWordLength(tempLongitude, flag);
            while (i < strlen(tempLongitude))
            {
                if (tempLongitude[i] == '.')
                {
                    dot++;
                }
                //check whether the latitude has more than 1 dot
                if (dot > 1)
                {
                    //wrong format

                    flag = 0;
                }
                if (((tempLongitude[i] >= '0') && (tempLongitude[i] <= '9')) || (tempLongitude[i] == '.'))
                {
                    //correct format
                    ;
                }
                else
                {
                    //incorrect format
                    flag = 0;
                }
                i++;
            }
        }
        if (flag)
        {
            latitude = atof(tempLatitude);
            longitude = atof(tempLongitude);
            pt = (Planner *)malloc(sizeof(Planner));
            if (pt == NULL)
            {
                fclose(fp);
                exit(-1);
            }
            else
            {
                p1 = head;

                //check whether the point is too close to previous point
                while (p1->next != NULL)
                {
                    //check whether the distance between new point and old point is less than 0.1
                    distance = getDistance(p1->latitude, latitude, p1->longitude, longitude);
                    if (distance < 0.1)
                    {
                        tooClose = 1;
                    }
                    p1 = p1->next;
                }

                if (tooClose == 0)
                {
                    //generate new struct to save data
                    strcpy(p->name, tempName);
                    p->latitude = latitude;
                    p->longitude = longitude;
                    p->next = pt;
                    p = p->next;
                    p->next = NULL;
                }
                else
                {
                    tooClose = 0;
                }
            }
        }
        else
        {
            //if file is invalid due to any reason, free all the data saved in ths file and return the old point
            printf("Invalid file.\n");
            freeAndQuit(pp);
            fclose(fp);
            return pp;
        }
        flag = 1;
    }

    //close the file and return the new point pointing to the newest struct
    fclose(fp);
    return p;
}
//show the data saved in linked list and the map
void showTarget(Planner *head)
{
    int la, lo;
    int i, j;
    Planner *p;
    p = head;
    int map[HEIGHT][WIDTH];
    //set all element to 0
    memset(map, 0, sizeof(map));

    if (head->next == NULL)
    {
        return;
    }

    while (p->next != NULL)
    {
        printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
        //mark the relative point t
        la = (int)((p->latitude) * HEIGHT / 100 + 0.5);
        lo = (int)((p->longitude) * WIDTH / 100 + 0.5);
        map[la][lo] = 1;
        p = p->next;
    }
    printf("\n");
    //print the whole map
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == 0)
            {
                printf("-");
            }
            else if (map[i][j] == 1)
            {
                printf("X");
            }
        }
        printf("\n");
    }
    printf("\n");
}

//check the words contained in file can be directly divided by 3 without any reminder
int checkWords(FILE *fp)
{
    int i = 0;
    int count = 0, valid = 1;
    char var[1024];

    fgets(var, 1024, fp);
    for (i = 0; i < strlen(var); i++)
    {
        if (var[i] != ' ')
        {
            count++;
            while (var[i] != ' ' && var[i] != '\0')
            {
                i++;
            }
        }
    }
    if ((count % 3) != 0)
    {
        valid = 0;
    }

    return valid;
}
//free the data saved in linked list which from an invalid file
void freeAndQuit(Planner *p)
{
    Planner *pt, *pp;
    pp = p;

    while (1)
    {
        if (p->next == NULL)
        {
            pp->next = NULL;
            return;
        }
        else
        {
            pt = p->next;
        }
        free(p);
        p = pt;
    }
}
//search the target by its name
void searchTarget(Planner *head)
{
    Planner *p;
    p = head;
    int flag = 0, i = 0;
    char c;
    char name[17];
    printf("Enter the name: ");
    while (1)
    {
        c = getchar();

        name[i] = c;
        if (name[0] == '\n')
        {
            return;
        }

        if (c == '\n')
        {
            break;
        }
        i++;
    }
    name[i] = '\0';
    name[16] = '\0';
    if (strlen(name) > 15)
    {
        return;
    }

    while (p->next != NULL)
    {

        if ((strcmp(p->name, name) == 0))
        {

            printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
            flag = 1;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        printf("Entry does not exist.\n");
    }
}
//find the target within the area entered by user
void plan(Planner *head)
{
    Planner *p;
    p = head;

    double pla, plo, ratio;
    printf("Enter predicted latitude: ");
    pla = inputADouble();
    if (pla < 0)
    {
        return;
    }
    printf("Enter predicted longitude: ");
    plo = inputADouble();
    if (plo < 0)
    {
        return;
    }
    printf("Enter ratio of damage zone: ");
    ratio = inputADouble();
    if (ratio < 0)
    {
        return;
    }
    while (p->next != NULL)
    {
        if (getDistance(p->latitude, pla, p->longitude, plo) < ratio)
        {
            printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
        }
        p = p->next;
    }
}
//calculate the distance between two point
double getDistance(double x1, double x2, double y1, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
//destroy the point and free them
Planner *destroy(Planner *head, Planner *oldp)
{
    Planner *p, *pp;
    p = head;
    int targetNumber = 0;
    double pla, plo, ratio;
    printf("Enter targeted latitude: ");
    pla = inputADouble();
    if (pla < 0)
    {
        return oldp;
    }
    printf("Enter targeted longitude: ");
    plo = inputADouble();
    if (plo < 0)
    {
        return oldp;
    }
    printf("Enter ratio of damage zone: ");
    ratio = inputADouble();
    if (ratio < 0)
    {
        return oldp;
    }
    while (p->next != NULL)
    {
        if (getDistance(p->latitude, pla, p->longitude, plo) < ratio)
        {
            targetNumber++;
        }
        p = p->next;
    }
    p = head;
    if (targetNumber > 0)
    {
        printf("%d target destroy.\n", targetNumber);
    }
    else
    {
        printf("No target aimed. Mission cancelled.\n");
    }

    while (p->next != NULL)
    {
        if (getDistance(p->latitude, pla, p->longitude, plo) < ratio)
        {
            printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
            pp = p->next;
            p->latitude = pp->latitude;
            p->longitude = pp->longitude;
            p->next = pp->next;
            strcpy(p->name, pp->name);

            free(pp);
        }
        else
        {
            p = p->next;
        }
    }
    return p;
}
//free the whole linked list
void freeAll(Planner *head)
{
    Planner *p, *pp;
    p = head;
    pp = head;

    while (p->next != NULL)
    {
        pp = p->next;
        free(p);
        p = pp;
    }
    free(p);
}
//check the length of each word(name or latitude or longitude) in file is less than 15 and check the value of the latitude and longitude which should in the range of 0~100
int checkWordLength(char str[16], int oldflag)
{

    int len = strlen(str);
    if (len > 15)
    {
        return 0;
    }

    if (atof(str) > 100 || atof(str) < 0)
    {
        return 0;
    }
    return oldflag;
}
//to let user input a string and then after checking it will be turned into double format
double inputADouble()
{
    char str[16];
    int dot = 0;
    scanf("%s", str);
    str[16] = '\0';

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
        {
            dot++;
        }
    }
    if (dot > 1)
    {
        return -1;
    }
    for (int i = 0; i < strlen(str); i++)
    {
        if (((str[i] >= '0') && (str[i] <= '9')) || str[i] == '.')
        {
            ;
        }
        else
        {
            return -1;
        }
    }
    return atof(str);
}
//a funcition used to receive the user's input and after checking its validation turned it into an integer
int menuInput()
{
    char numbers[2], c, ch;
    int input;
    scanf("%1s", numbers);
    c = getchar();

    if (c != '\n')
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
        return -1;
    }
    input = atoi(numbers);
    if (input < 1 || input > 6)
    {
        return -1;
    }
    else
    {
        return input;
    }
}
