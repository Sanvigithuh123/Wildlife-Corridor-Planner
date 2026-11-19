
#include <stdio.h>
#include "export.h"

int exportCSV(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    FILE *fp = fopen("output/network.csv", "w");

    if (fp == NULL) {
        printf("Cannot create CSV output.\n");
        return -1;
    }

    fprintf(
        fp,
        "source,destination,cost,capacity,available\n"
    );

    for (int i = 0; i < corridorCount; i++) {
        fprintf(
            fp,
            "%d,%d,%d,%d,%d\n",
            corridors[i].source,
            corridors[i].destination,
            corridors[i].cost,
            corridors[i].capacity,
            corridors[i].available
        );
    }

    fclose(fp);

    printf("Exported: output/network.csv\n");
    (void)habitats;
    (void)habitatCount;

    return 0;
}

int exportJSON(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    FILE *fp = fopen("output/network.json", "w");

    if (fp == NULL) {
        printf("Cannot create JSON output.\n");
        return -1;
    }

    fprintf(fp, "{\n  \"habitats\": [\n");

    for (int i = 0; i < habitatCount; i++) {
        fprintf(
            fp,
            "    {\"id\": %d, \"name\": \"%s\", "
            "\"population\": %d, \"threat\": %d}%s\n",
            habitats[i].id,
            habitats[i].name,
            habitats[i].population,
            habitats[i].threatLevel,
            i == habitatCount - 1 ? "" : ","
        );
    }

    fprintf(fp, "  ],\n  \"corridors\": [\n");

    for (int i = 0; i < corridorCount; i++) {
        fprintf(
            fp,
            "    {\"source\": %d, \"destination\": %d, "
            "\"cost\": %d, \"capacity\": %d, "
            "\"available\": %d}%s\n",
            corridors[i].source,
            corridors[i].destination,
            corridors[i].cost,
            corridors[i].capacity,
            corridors[i].available,
            i == corridorCount - 1 ? "" : ","
        );
    }

    fprintf(fp, "  ]\n}\n");
    fclose(fp);

    printf("Exported: output/network.json\n");
    return 0;
}
