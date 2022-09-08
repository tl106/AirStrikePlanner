//20124921 shytl1 Ting Lou
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//define the width and height of the map
#define WIDTH 80
#define HEIGHT 37

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
int checkWordLength1(char str[], int oldflag);
double inputADouble();
void inputFileName(char fileName[]);
int menuInput();
int checkMap(int map[HEIGHT][WIDTH], int la, int lo, int len);
int main()
{
    Planner *head, *p;
    head = (Planner *)malloc(sizeof(Planner));
    head->next = NULL;
    p = head;
    int input;
    //print the menu
    printf("1) Load a target file\n2) Show current targets\n3) Search a target\n4) Plan an airstrike\n5) Execute an airstrike\n6) Quit\n");
    //keep asking user to input until get the valid input, only consist of integer 1~6, anything beyond this range is invalid(e.g. "1 "," 1","01" is invalid), however "\n1" is seemed as valid input
    while (1)
    {

        printf("Option: ");
        //ask for input
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
                //load file
                p = loadFile(p, head);
            }
            else if (input == 2)
            {
                //show the information of exist target
                showTarget(head);
            }
            else if (input == 3)
            {
                //search the target by its name
                searchTarget(head);
            }
            else if (input == 4)
            {
                //search the target by its position
                plan(head);
            }
            else if (input == 5)
            {
                //destroy the target by its position
                p = destroy(head, p);
            }
            else if (input == 6)
            {
                //free all the memory remaining in heap
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
    int i = 0, f, flag = 1, dot = 0, valid, tooClose = 0, count = 0;
    FILE *fp;
    char fileName[40];
    char tempName[16], tempLatitude[16], tempLongitude[16];
    double latitude, longitude, distance;
    char ch;
    // int targetted = 0;
    Planner *pt, *pp, *p1;
    //remain the the address of the last node in the link list
    pp = p;

    printf("Enter a target file: ");
    //ask user to input the file name which store the information of the new target(s)
    inputFileName(fileName);
    //when there is no file named as user input, prompt invalid warning
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("Invalid file.\n");
        return p;
    }
    //check the words contained in file can be directly divided by 3 without any reminder
    valid = checkWords(fp);
    //reset the pointer inside the file to the begining
    rewind(fp);
    //deal with the situation that the file which only a '\n' in it.
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }
    if (count > 0)
    {
        valid = 0;
    }

    if (valid == 0)
    {
        //invalid
        printf("Invalid file.\n");
        fclose(fp);
        return p;
    }
    //get the pointer back to the begignning of the file
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
            //check the length whether it is less than 16
            flag = checkWordLength1(tempName, flag);
            //check whether the value has invalid character
            while (i < strlen(tempName))
            {
                if (((tempName[i] >= '0') && (tempName[i] <= '9')) || ((tempName[i] >= 'a') && (tempName[i] <= 'z')) || ((tempName[i] >= 'A') && (tempName[i] <= 'Z')))
                {

                    ;
                }
                else
                {
                    //set flag to 0 to show it is a wrong file
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
            //check both the length and the value of the number(whether it is within the range of 0~100)
            flag = checkWordLength(tempLatitude, flag);
            while (i < strlen(tempLatitude))
            {
                //check whether the latitude has more than 1 dot(e.g. 23.32.32 is invalid)
                if (tempLatitude[i] == '.')
                {
                    dot++;
                }

                if (dot > 1)
                {
                    //invalid file
                    flag = 0;
                }

                if (((tempLatitude[i] >= '0') && (tempLatitude[i] <= '9')) || (tempLatitude[i] == '.'))
                {
                    ;
                }
                else
                {
                    //invalid file
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
            //check both the length and the value of the number(whether it is within the range of 0~100)
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
                    //invalid file
                    ;
                }
                else
                {
                    //invalid file
                    flag = 0;
                }
                i++;
            }
        }
        //if flag==1, then it is a valid file, otherwise it is an invalid file
        if (flag)
        {
            //transfer the string into double
            latitude = atof(tempLatitude);
            longitude = atof(tempLongitude);
            //get the memory in heap
            pt = (Planner *)malloc(sizeof(Planner));
            //if the heap is run out of the memory, the profgram will exit with argument -1
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
                //if the new target has enough distance with the targets which has already existenced, add it in link list
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
                    //otherwise free all the memory which has already been saved in link list from this file
                    free(pt);
                    //reset the tooClose variable to 0
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
    int namePosition;
    int len;
    Planner *p;
    p = head;
    //initial the map
    int map[HEIGHT][WIDTH];
    //set all element to 0 first
    memset(map, 0, sizeof(map));
    //if the link list has no data saved in it, directly return to the main function
    if (head->next == NULL)
    {
        return;
    }
    //set some basic point in map, like the coordinate axis and scale(e.g. 0,20,40,60,80,100)
    //different value related to different prompt:
    /*   value          relative prompt in cmd
    *      1~9          the same number of the targets in that point  
    *      9~48         when the targets number is more than 9, it will show 'X'
    *        0          ' '
    *       -1          '+'     
    *       -2          '|'
    *       -3          '-'
    *       -4          'v'
    *       -5          '>'
    *       -6          ' '
    *       -7          '0'
    */
    map[0][17] = 2;
    map[0][18] = -7;
    map[0][33] = -7;
    map[0][48] = -7;
    map[0][63] = -7;
    map[0][32] = 4;
    map[0][47] = 6;
    map[0][62] = 8;
    map[0][77] = 1;
    map[0][78] = -7;
    map[0][79] = -7;
    map[36][0] = 1;
    map[36][1] = -7;
    map[36][2] = -7;
    map[0][2] = -7;
    for (i = 1; i < 5; i++)
    {
        map[7 * i + 1][1] = i * 2;
        map[7 * i + 1][2] = -7;
    }

    for (i = 0; i < 36; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (map[i][j] == 0)
            {
                map[i][j] = -6;
            }
        }
    }
    for (i = 0; i < 80; i++)
    {
        if (map[0][i] == 0)
        {
            map[0][i] = -6;
        }
        if (i > 2)
        {
            map[1][i] = -3;
        }
    }
    for (i = 1; i < 37; i++)
    {
        map[i][3] = -2;
    }
    map[1][79] = -5;
    map[36][3] = -4;
    map[1][3] = -1;
    //put the target to its position and the number of the value show the quantity of the targets in that point
    while (p->next != NULL)
    {
        //print the basic information of each target
        printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
        //mark the relative point t
        la = (int)((p->latitude) * (HEIGHT - 2) / 101.45 + 0.5) + 2;
        lo = (int)((p->longitude) * (WIDTH - 4) / 100.67 + 0.5) + 4;
        map[la][lo]++;
        p = p->next;
    }
    //let p point to the begining of the link list
    p = head;
    //put the name of each target to appropriate position
    while (p->next != NULL)
    {
        len = strlen(p->name);
        la = (int)((p->latitude) * (HEIGHT - 2) / 101.45 + 0.5) + 2;
        lo = (int)((p->longitude) * (WIDTH - 4) / 100.67 + 0.5) + 4;
        //find the appropriate position to put the name of the target
        namePosition = checkMap(map, la, lo, strlen(p->name));
        //namePosition ==1 means the name will below the target;
        //namePosition ==2 means the name will on the right of the target;
        //namePosition ==3 means the name will above the target;
        //namePosition ==4 means the name will on the left of the target;
        //if there is no space to put the name, then newPosition==0, the specific name will not been shown on the map

        if (namePosition == 1)
        {
            for (i = 0; i < len; i++)
            {
                map[la + 1][lo + (1 - len) / 2 + i] = p->name[i];
            }
        }
        else if (namePosition == 2)
        {
            for (i = 0; i < len; i++)
            {
                map[la][lo + i + 2] = p->name[i];
            }
        }
        else if (namePosition == 3)
        {
            for (i = 0; i < len; i++)
            {
                map[la - 1][lo + (1 - len) / 2 + i] = p->name[i];
            }
        }
        else if (namePosition == 4)
        {
            for (i = 0; i < len; i++)
            {
                map[la][lo - i - 2] = p->name[len - 1 - i];
            }
        }
        else
        {
            //there is no space to put the name of the target.
            ;
        }
        p = p->next;
    }

    printf("\n");
    // when the position has target, and it will print 'X' when the position has more than 9 targets
    //different value will have different prompt, please see the form above for details
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == 0)
            {
                printf(" ");
            }
            else if (map[i][j] > 0 && map[i][j] < 10)
            {
                printf("%d", map[i][j]);
            }
            else if (map[i][j] > 9 && map[i][j] < 48)
            {
                printf("X");
            }
            else if (map[i][j] >= 48)
            {
                printf("%c", map[i][j]);
            }
            else if (map[i][j] == -1)
            {
                printf("+");
            }
            else if (map[i][j] == -2)
            {
                printf("|");
            }
            else if (map[i][j] == -3)
            {
                printf("-");
            }
            else if (map[i][j] == -4)
            {
                printf("V");
            }
            else if (map[i][j] == -5)
            {
                printf(">");
            }
            else if (map[i][j] == -6)
            {
                printf(" ");
            }
            else if (map[i][j] == -7)
            {
                printf("0");
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
    int back = 0;
    int flag = 0, i = 0;
    char c;
    char name[17];
    printf("Enter the name: ");
    while (1)
    {
        c = getchar();
        if (i == 16)
        {
            //when user input 16 char, show warning
            printf("Entry does not exist.\n");
            break;
        }
        name[i] = c;
        //if user just input a '\n', return to the main function
        if (name[0] == '\n')
        {
            return;
        }
        //continue until user input '\n'
        if (c == '\n')
        {
            back = 1;
            break;
        }
        i++;
    }
    //set the last bit to '\0'
    name[i] = '\0';
    //if user input more that 16 bit(including 15 normal character and '\n'), then clear the buffer
    if (back == 0)
    {
        while ((c = getchar()) != '\n')
            ;
    }
    //if strlen is more than 15, return to the main function
    if (strlen(name) > 15)
    {
        return;
    }
    //Traversing the list to find the target
    while (p->next != NULL)
    {

        if ((strcmp(p->name, name) == 0))
        {

            printf("%s %lf %lf\n", p->name, p->latitude, p->longitude);
            flag = 1;
        }
        p = p->next;
    }
    //if no name can match the input, show warning
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
    //ask the user to input
    pla = inputADouble();
    //if the value is not a double within the range of 0~100, return to the main function
    if (pla < 0 || pla > 100)
    {
        return;
    }
    printf("Enter predicted longitude: ");
    plo = inputADouble();
    if (plo < 0 || plo > 100)
    {
        return;
    }
    printf("Enter ratio of damage zone: ");
    ratio = inputADouble();
    //if the value is not a double or less than 0, return to the main function
    if (ratio < 0)
    {
        return;
    }
    //Traversing the list to find the target
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
    if (pla < 0 || pla > 100)
    {
        return oldp;
    }
    printf("Enter targeted longitude: ");
    plo = inputADouble();
    if (plo < 0 || plo > 100)
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
    //if no target can be match, show warning，otherwise show the number if target in the area
    if (targetNumber > 0)
    {
        printf("%d target destroy.\n", targetNumber);
    }
    else
    {
        printf("No target aimed. Mission cancelled.\n");
    }
    //free the memory and adjust the tail of the link list
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
    //return the current tail to the main function
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

//check the length of each word(name) in file which is less than 15
int checkWordLength1(char str[16], int oldflag)
{
    int len = strlen(str);
    if (len > 15)
    {
        return 0;
    }
    return oldflag;
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
//if user input a number which has more than 15 bit but it is within the 0~100(e.g. 23.32222222222222222222222222222222222222222222), it will be seemed as valid value, and the program will continue to ask the user to input or give the result.
double inputADouble()
{
    char str[16];
    int dot = 0;
    scanf("%16s", str);
    str[16] = '\0';
    int i;
    char c;
    //check the number of the '.'
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
        {
            dot++;
        }
    }
    //if the value have more than one '.',clear the buffer and return to the main function
    if (dot > 1)
    {
        while ((c = getchar()) != '\n')
            ;
        return -1;
    }
    //check whether each bit is correct
    for (i = 0; i < strlen(str); i++)
    {
        if (((str[i] >= '0') && (str[i] <= '9')) || str[i] == '.')
        {
            ;
        }
        else
        {
            while ((c = getchar()) != '\n')
                ;
            return -1;
        }
    }
    while ((c = getchar()) != '\n')
        ;
    //transfer the string into double and return it
    return atof(str);
}

//to let user input a string
void inputFileName(char fileName[40])
{
    char str[40];
    char c;
    int i = 0;
    int flag = 0;

    while (1)
    {
        c = getchar();
        if (i == 39)
        {
            break;
        }
        str[i] = c;
        if (c == '\n')
        {
            flag = 1;
            break;
        }
        i++;
    }
    str[i] = '\0';
    str[39] = '\0';
    if (flag == 0)
    {
        //clear the buffer
        while ((c = getchar()) != '\n')
            ;
    }

    if (strlen(str) > 38)
    {
        return;
    }
    //update the string
    strcpy(fileName, str);
}

//a funcition used to receive the user's input and after checking its validation turned it into an integer
int menuInput()
{
    char c1, c2, ch;

    c1 = getchar();
    while (1)
    {
        if (c1 == '\n')
        {
            c1 = getchar();
            continue;
        }
        if (c1 < 49 || c1 > 54)
        {
            while ((ch = getchar()) != '\n')
                ;
            return -1;
        }
        break;
    }

    c2 = getchar();
    //if the second input character is not equal to '\n', clear the rest buffer and return to the main function
    if (c2 != '\n')
    {
        while ((ch = getchar()) != '\n')
            ;
        return -1;
    }
    //minus by 48 to transfer from ascii code to integer
    return c1 - 48;
}
//check which side have space to put the name of the target,the priority is below => right => above => left
int checkMap(int map[HEIGHT][WIDTH], int la, int lo, int len)
{
    //available==1 means current side has space to put the name of the target
    int i, available = 1;
    //check whether there is enough space below the target
    if (la < HEIGHT)
    {
        for (i = (-len + 1) / 2 - 1; i < (-len + 1) / 2 + len + 1; i++)
        {
            if ((map[la + 1][lo + i] != 0) || lo + i <= 3 || lo + i >= WIDTH)
            {

                available = 0;
            }
        }
        if (available == 1)
        {
            return 1;
        }
        else
        {
            available = 1;
        }
    }

    //check whether there is enough space to the right of the target
    if (lo < WIDTH)
    {
        for (i = 0; i < len + 2; i++)
        {
            if (map[la][lo + i + 1] != 0 || lo + i + 1 >= WIDTH)
            {
                available = 0;
            }
        }
        if (available == 1)
        {
            return 2;
        }
        else
        {
            available = 1;
        }
    }
    //check whether there is enough space above the target
    if (la > 2)
    {
        for (i = (-len + 1) / 2 - 1; i < (-len + 1) / 2 + len + 1; i++)
        {
            if ((map[la - 1][lo + i] != 0) || lo + i <= 3 || lo + i >= WIDTH)
            {
                available = 0;
            }
        }
        if (available == 1)
        {
            return 3;
        }
        else
        {
            available = 1;
        }
    }

    //check whether there is enough space to the left of the target
    if (lo > 4)
    {
        for (i = 0; i < len + 2; i++)
        {
            if (map[la][lo - i - 1] != 0 || lo - i - 1 <= 4)
            {
                available = 0;
            }
        }
        if (available == 1)
        {
            return 4;
        }
    }
    return 0;
}
