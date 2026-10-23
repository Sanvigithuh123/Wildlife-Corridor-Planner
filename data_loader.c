
#include <stdio.h>
#include <string.h>
#include "data_loader.h"

int loadHabitats(
    const char *filename,
    Habitat habitats[],
    int maxHabitats
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening %s\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    // Skip the CSV header
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {

        if (count >= maxHabitats) {
            printf("Too many habitats!\n");
            fclose(fp);
            return -1;
        }

        Habitat h;

        int fields = sscanf(
            line,
            "%d,%49[^,],%d,%d",
            &h.id,
            h.name,
            &h.population,
            &h.threatLevel
        );

        if (fields != 4 ||
            h.id < 0 ||
            h.population < 0 ||
            h.threatLevel < 0) {

            printf("Invalid habitat record: %s", line);
            fclose(fp);
            return -1;
        }

        // IDs must be unique
        for (int i = 0; i < count; i++) {
            if (habitats[i].id == h.id) {
                printf("Duplicate habitat ID: %d\n", h.id);
                fclose(fp);
                return -1;
            }
        }

        habitats[count] = h;
        count++;
    }

    fclose(fp);
    return count;
}

int loadCorridors(
    const char *filename,
    Corridor corridors[],
    int maxCorridors
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening %s\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    // Skip header
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {

        if (count >= maxCorridors) {
            printf("Too many corridors!\n");
            fclose(fp);
            return -1;
        }

        Corridor c;

        int fields = sscanf(
            line,
            "%d,%d,%d,%d,%d",
            &c.source,
            &c.destination,
            &c.cost,
            &c.capacity,
            &c.available
        );

        if (fields != 5 ||
            c.source < 0 ||
            c.destination < 0 ||
            c.source == c.destination ||
            c.cost < 0 ||
            c.capacity < 0 ||
            (c.available != 0 && c.available != 1)) {

            printf("Invalid corridor record: %s", line);
            fclose(fp);
            return -1;
        }

        corridors[count] = c;
        count++;
    }

    fclose(fp);
    return count;
}
