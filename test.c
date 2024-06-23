#include <stdio.h>
#define MAX 50

void drawBar(int maxBarHeight, int barWidth, float payDays[], int start, int end, char listFiles[][MAX]);
int countDigits(int n);
int main() {
    float payDays[] = {100000, 150000, 200000, 250000};
    char listFiles[][MAX] = {"File1", "File2", "File3", "File4"};
    int start = 0;
    int end = 4;
    int maxBarHeight = 10;
    int barWidth = 2;
    
    drawBar(maxBarHeight, barWidth, payDays, start, end, listFiles);
    
    return 0;
}
void drawBar(int maxBarHeight, int barWidth, float payDays[], int start, int end, char listFiles[][MAX])
{
    for (int i = 0; i < maxBarHeight; i++)
    {
        for (int j = start; j < end; j++)
        {
            if (payDays[j] / 120000 * maxBarHeight >= maxBarHeight - i)
            {
                printf("\t");
                for (int w = 0; w < barWidth; w++)
                {
                    printf("█");
                }
            }
            else
            {
                for (int w = 0; w < barWidth; w++)
                {
                    printf(" ");
                }
            }
            printf("         "); // Space between bars
        }
        printf("\n");
    }
    for (int i = start; i < end; i++)
    {
        printf("      %s", listFiles[i]);
    }
    printf("\n");
    for (int i = 0; i < maxBarHeight + 2; i++)
    {
        printf("│");
        printf("\033[4D");
        printf("\033[1A");
    }
    printf("\t");
    for (int j = start; j < end; j++)
    {
        float currentBarHeight = payDays[j] / 120000 * maxBarHeight;
        if((int)currentBarHeight - maxBarHeight == 0)
        {
            printf("%.0f", payDays[j]);
            printf("\033[%dD", countDigits((int)payDays[j]));
            printf("      ");
            continue;
        }
        printf("\033[%dB%.0f",maxBarHeight - (int)currentBarHeight, payDays[j]);
        printf("\033[%dD", countDigits((int)payDays[j]));
        printf("\033[%dA", maxBarHeight - (int)currentBarHeight);
        printf("      ");
    }
}
int countDigits(int n)
{
    int count = 0;
    while (n != 0)
    {
        n /= 10;
        count++;
    }
    return count;
}
