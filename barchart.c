#include <stdio.h>

#define MAX_BARS 5
#define MAX_BAR_LENGTH 50

// Function to draw a single bar
void drawBar(int length) {
    for (int i = 0; i < length; i++) {
        printf("█"); // Using a block character for the bar
    }
    printf("\n");
    printf("\n");
}

// Function to draw the entire bar chart
void drawBarChart(int data[], int size) {
    // Find the maximum value to scale the bar lengths
    int maxVal = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] > maxVal) {
            maxVal = data[i];
        }
    }

    // Draw each bar, scaled to the maximum length
    for (int i = 0; i < size; i++) {
        printf("Bar %d: ", i + 1);
        int length = (data[i] * MAX_BAR_LENGTH) / maxVal; // Scale the length
        drawBar(length);
    }
}

int main() {
    int data[MAX_BARS] = {10, 20, 30, 40, 50}; // Example data

    drawBarChart(data, MAX_BARS);

    return 0;
}