#include <iostream>
#include <iomanip>
using namespace std;

#define N 4
#define INF 999

// Function to display routing tables
void displayRoutingTables(int dist[N][N], int nextHop[N][N])
{
    for (int i = 0; i < N; i++)
    {
        cout << "\nRouting Table for Router " << i + 1 << endl;
        cout << "-----------------------------------" << endl;
        cout << left << setw(15) << "Destination"
             << setw(10) << "Cost"
             << setw(10) << "Next Hop" << endl;

        for (int j = 0; j < N; j++)
        {
            cout << left << setw(15) << j + 1;

            if (dist[i][j] == INF)
                cout << setw(10) << "INF";
            else
                cout << setw(10) << dist[i][j];

            if (nextHop[i][j] == -1)
                cout << setw(10) << "-";
            else
                cout << setw(10) << nextHop[i][j] + 1;

            cout << endl;
        }
    }
}

// Function to display the cost matrix
void displayCostMatrix(int cost[N][N])
{
    cout << "\nInitial Network Cost Matrix" << endl;
    cout << "---------------------------" << endl;

    cout << setw(8) << "";

    for (int i = 0; i < N; i++)
        cout << setw(8) << i + 1;

    cout << endl;

    for (int i = 0; i < N; i++)
    {
        cout << setw(8) << i + 1;

        for (int j = 0; j < N; j++)
        {
            if (cost[i][j] == INF)
                cout << setw(8) << "INF";
            else
                cout << setw(8) << cost[i][j];
        }

        cout << endl;
    }
}

// Function to display shortest path
void displayShortestPath(int source, int destination,
                         int dist[N][N], int nextHop[N][N])
{
    cout << "\nShortest Path" << endl;
    cout << "-------------" << endl;

    if (dist[source][destination] == INF)
    {
        cout << "No path exists between Router "
             << source + 1 << " and Router "
             << destination + 1 << endl;
        return;
    }

    cout << "Source      : Router " << source + 1 << endl;
    cout << "Destination : Router " << destination + 1 << endl;
    cout << "Minimum Cost: " << dist[source][destination] << endl;

    cout << "Path        : Router " << source + 1;

    int current = source;

    while (current != destination)
    {
        current = nextHop[current][destination];

        if (current == -1)
        {
            cout << "\nPath cannot be reconstructed." << endl;
            return;
        }

        cout << " -> Router " << current + 1;
    }

    cout << endl;
}

int main()
{
    int cost[N][N];
    int dist[N][N];
    int nextHop[N][N];

    // -----------------------------------------
    // Step 1: Read the cost matrix
    // -----------------------------------------

    cout << "DISTANCE VECTOR ROUTING ALGORITHM" << endl;
    cout << "=================================" << endl;

    cout << "\nEnter the cost matrix for "
         << N << " routers." << endl;

    cout << "Enter " << INF << " for no direct connection.\n"
         << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "Enter costs from Router " << i + 1 << ": ";

        for (int j = 0; j < N; j++)
        {
            cin >> cost[i][j];
        }
    }

    // -----------------------------------------
    // Step 2: Display initial cost matrix
    // -----------------------------------------

    displayCostMatrix(cost);

    // -----------------------------------------
    // Step 3: Initialize distance and nextHop
    // -----------------------------------------

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Initially distance is equal to cost
            dist[i][j] = cost[i][j];

            // If source and destination are same
            if (i == j)
            {
                nextHop[i][j] = i;
            }

            // If there is a direct connection
            else if (cost[i][j] != INF)
            {
                nextHop[i][j] = j;
            }

            // If there is no connection
            else
            {
                nextHop[i][j] = -1;
            }
        }
    }

    // -----------------------------------------
    // Step 4: Display initial routing tables
    // -----------------------------------------

    cout << "\n\nINITIAL ROUTING TABLES" << endl;
    cout << "======================" << endl;

    displayRoutingTables(dist, nextHop);

    // -----------------------------------------
    // Step 5: Distance Vector Updates
    // -----------------------------------------

    int updated;

    do
    {
        updated = 0;

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    // Check if both paths exist
                    if (dist[i][k] != INF &&
                        dist[k][j] != INF)
                    {
                        // Check if going through router k
                        // gives a shorter path
                        if (dist[i][j] >
                            dist[i][k] + dist[k][j])
                        {
                            // Update minimum distance
                            dist[i][j] =
                                dist[i][k] + dist[k][j];

                            // Update next hop
                            nextHop[i][j] =
                                nextHop[i][k];

                            // A change occurred
                            updated = 1;
                        }
                    }
                }
            }
        }

    } while (updated == 1);

    // -----------------------------------------
    // Step 6: Display final routing tables
    // -----------------------------------------

    cout << "\n\nFINAL ROUTING TABLES" << endl;
    cout << "====================" << endl;

    displayRoutingTables(dist, nextHop);

    // -----------------------------------------
    // Step 7: Find shortest path
    // -----------------------------------------

    int source, destination;

    cout << "\nEnter source router (1-" << N << "): ";
    cin >> source;

    cout << "Enter destination router (1-" << N << "): ";
    cin >> destination;

    // Convert router number to array index
    source--;
    destination--;

    // -----------------------------------------
    // Step 8: Display shortest path
    // -----------------------------------------

    displayShortestPath(source, destination, dist, nextHop);

    return 0;
}