/*
 * ================================================
 *  UNIT CONVERTER & FORMULA SOLVER
 *  1st Semester C Project
 *
 *  Concepts: pointers, structs, linked list,
 *            function pointers, file I/O, math.h
 * ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979

/* =========================================
   LINKED LIST FOR HISTORY
   ========================================= */

struct HistoryNode {
    char result[300];
    struct HistoryNode *next;
};

struct HistoryNode *history_head = NULL;

void add_to_history(const char *text) {
    struct HistoryNode *node = (struct HistoryNode *)malloc(sizeof(struct HistoryNode));
    /* check malloc here, if memory allocation fails this can crash straight away */
    strncpy(node->result, text, 299);
    /* strncpy may leave string without '\0' at the end, so set last char manually */
    node->next = history_head;
    history_head = node;
}

void show_history() {
    printf("\n===== CALCULATION HISTORY =====\n");
    if (history_head == NULL) {
        printf("No history yet.\n");
        return;
    }
    struct HistoryNode *current = history_head;
    int count = 1;
    while (current != NULL) {
        printf("%d. %s\n", count, current->result);
        current = current->next;
        count++;
    }
    printf("================================\n\n");
}

void save_history() {
    FILE *fp = fopen("history.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    struct HistoryNode *current = history_head;
    while (current != NULL) {
        fprintf(fp, "%s\n", current->result);
        current = current->next;
    }
    fclose(fp);
    printf("History saved to history.txt\n\n");
    /* nice extra touch would be auto-save on exit too, users may forget menu option 4 */
}

void free_history() {
    struct HistoryNode *current = history_head;
    while (current != NULL) {
        struct HistoryNode *temp = current;
        current = current->next;
        free(temp);
    }
}

/* =========================================
   UNIT CONVERTER
   ========================================= */

/* Function pointer type for conversions */
typedef double (*ConvertFunc)(double);

/* Conversion functions */
double km_to_mile(double x)   { return x * 0.621371; }
double mile_to_km(double x)   { return x * 1.60934;  }
double meter_to_ft(double x)  { return x * 3.28084;  }
double ft_to_meter(double x)  { return x / 3.28084;  }
double cm_to_inch(double x)   { return x * 0.393701; }
double inch_to_cm(double x)   { return x * 2.54;     }

double cel_to_fahr(double x)  { return (x * 9.0 / 5.0) + 32.0; }
double fahr_to_cel(double x)  { return (x - 32.0) * 5.0 / 9.0; }
double cel_to_kelvin(double x){ return x + 273.15; }
double kelvin_to_cel(double x){ return x - 273.15; }

double kg_to_pound(double x)  { return x * 2.20462; }
double pound_to_kg(double x)  { return x / 2.20462; }
double gram_to_oz(double x)   { return x * 0.035274; }
double oz_to_gram(double x)   { return x / 0.035274; }

double kmh_to_mph(double x)   { return x * 0.621371; }
double mph_to_kmh(double x)   { return x * 1.60934;  }
double ms_to_kmh(double x)    { return x * 3.6;      }
double kmh_to_ms(double x)    { return x / 3.6;      }

/* Struct to store conversion info */
struct Conversion {
    char from_unit[20];
    char to_unit[20];
    ConvertFunc convert;   /* function pointer */
    /* these unit names can be const char* too, that makes the intent cleaner */
};

/* Table of all conversions */
struct Conversion conv_list[] = {
    /* LENGTH */
    {"km",      "miles",   km_to_mile },
    {"miles",   "km",      mile_to_km },
    {"meters",  "feet",    meter_to_ft},
    {"feet",    "meters",  ft_to_meter},
    {"cm",      "inches",  cm_to_inch },
    {"inches",  "cm",      inch_to_cm },
    /* TEMPERATURE */
    {"Celsius", "Fahrenheit", cel_to_fahr  },
    {"Fahrenheit","Celsius",  fahr_to_cel  },
    {"Celsius", "Kelvin",     cel_to_kelvin},
    {"Kelvin",  "Celsius",    kelvin_to_cel},
    /* MASS */
    {"kg",      "pounds",  kg_to_pound},
    {"pounds",  "kg",      pound_to_kg},
    {"grams",   "ounces",  gram_to_oz },
    {"ounces",  "grams",   oz_to_gram },
    /* SPEED */
    {"km/h",    "mph",     kmh_to_mph },
    {"mph",     "km/h",    mph_to_kmh },
    {"m/s",     "km/h",    ms_to_kmh  },
    {"km/h",    "m/s",     kmh_to_ms  },
};

int total_conversions = 18;
/* this should come from sizeof(conv_list) / sizeof(conv_list[0]), otherwise easy to forget later */

void unit_converter_menu() {
    int i, choice;
    double value, result;
    char log[300];

    printf("\n");
    printf("============================================\n");
    printf("            UNIT CONVERTER\n");
    printf("============================================\n");
    printf("  No.  From          -->   To\n");
    printf("--------------------------------------------\n");

    /* Length */
    printf("  --- LENGTH ---\n");
    for (i = 0; i < 6; i++)
        printf("  %-3d  %-14s -->   %s\n", i+1, conv_list[i].from_unit, conv_list[i].to_unit);

    /* Temperature */
    printf("  --- TEMPERATURE ---\n");
    for (i = 6; i < 10; i++)
        printf("  %-3d  %-14s -->   %s\n", i+1, conv_list[i].from_unit, conv_list[i].to_unit);

    /* Mass */
    printf("  --- MASS ---\n");
    for (i = 10; i < 14; i++)
        printf("  %-3d  %-14s -->   %s\n", i+1, conv_list[i].from_unit, conv_list[i].to_unit);

    /* Speed */
    printf("  --- SPEED ---\n");
    for (i = 14; i < 18; i++)
        printf("  %-3d  %-14s -->   %s\n", i+1, conv_list[i].from_unit, conv_list[i].to_unit);

    printf("============================================\n");
    printf("Enter conversion number (1 to 18): ");
    scanf("%d", &choice);
    /* scanf return value should be checked, text input can break the menu badly */

    if (choice < 1 || choice > 18) {
        printf("Invalid choice! Please enter 1 to 18.\n\n");
        return;
    }

    printf("Enter value in %s: ", conv_list[choice-1].from_unit);
    scanf("%lf", &value);
    /* same thing here, better to use fgets + strtod if you want proper input handling */

    /* Call function using pointer */
    result = conv_list[choice-1].convert(value);

    printf("\n--------------------------------------------\n");
    printf("  RESULT:  %.4lf %s  =  %.4lf %s\n",
           value, conv_list[choice-1].from_unit,
           result, conv_list[choice-1].to_unit);
    printf("--------------------------------------------\n\n");

    /* Save to linked list */
    sprintf(log, "%.4lf %s = %.4lf %s",
            value, conv_list[choice-1].from_unit,
            result, conv_list[choice-1].to_unit);
    /* snprintf would be safer here */
    add_to_history(log);
}

/* =========================================
   FORMULA SOLVER
   ========================================= */

void formula_kinematics() {
    int choice;
    double u, v, a, t, s;
    char log[300];

    printf("\n--- KINEMATICS FORMULAS ---\n");
    printf("1. Final velocity     v = u + a*t\n");
    printf("2. Displacement       s = u*t + 0.5*a*t^2\n");
    printf("3. Velocity squared   v^2 = u^2 + 2*a*s\n");
    printf("Choose formula (1-3): ");
    scanf("%d", &choice);
    /* every scanf in this file really needs validation, not just a few of them */

    if (choice == 1) {
        printf("Enter initial velocity u (m/s): "); scanf("%lf", &u);
        printf("Enter acceleration a (m/s^2):   "); scanf("%lf", &a);
        printf("Enter time t (seconds):          "); scanf("%lf", &t);
        v = u + a * t;
        printf("\nRESULT: Final velocity v = %.4lf m/s\n\n", v);
        sprintf(log, "Kinematics v=u+at: u=%.2lf, a=%.2lf, t=%.2lf  =>  v=%.4lf m/s", u, a, t, v);

    } else if (choice == 2) {
        printf("Enter initial velocity u (m/s): "); scanf("%lf", &u);
        printf("Enter acceleration a (m/s^2):   "); scanf("%lf", &a);
        printf("Enter time t (seconds):          "); scanf("%lf", &t);
        s = u*t + 0.5*a*t*t;
        printf("\nRESULT: Displacement s = %.4lf meters\n\n", s);
        sprintf(log, "Kinematics s=ut+0.5at^2: u=%.2lf, a=%.2lf, t=%.2lf  =>  s=%.4lf m", u, a, t, s);

    } else if (choice == 3) {
        printf("Enter initial velocity u (m/s): "); scanf("%lf", &u);
        printf("Enter acceleration a (m/s^2):   "); scanf("%lf", &a);
        printf("Enter displacement s (meters):   "); scanf("%lf", &s);
        /* before sqrt, check if u*u + 2*a*s is negative or you get nan */
        v = sqrt(u*u + 2*a*s);
        printf("\nRESULT: Final velocity v = %.4lf m/s\n\n", v);
        sprintf(log, "Kinematics v^2=u^2+2as: u=%.2lf, a=%.2lf, s=%.2lf  =>  v=%.4lf m/s", u, a, s, v);

    } else {
        printf("Invalid choice.\n\n");
        return;
    }
    add_to_history(log);
}

void formula_ohmslaw() {
    int choice;
    double V, I, R;
    char log[300];

    printf("\n--- OHM'S LAW  (V = I x R) ---\n");
    printf("What do you want to find?\n");
    printf("1. Voltage   V  (given I and R)\n");
    printf("2. Current   I  (given V and R)\n");
    printf("3. Resistance R (given V and I)\n");
    printf("Choose (1-3): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter current I (Amperes):   "); scanf("%lf", &I);
        printf("Enter resistance R (Ohms):   "); scanf("%lf", &R);
        V = I * R;
        printf("\nRESULT: Voltage V = %.4lf Volts\n\n", V);
        sprintf(log, "Ohm's Law V=IR: I=%.2lf A, R=%.2lf Ohm  =>  V=%.4lf V", I, R, V);

    } else if (choice == 2) {
        printf("Enter voltage V (Volts):     "); scanf("%lf", &V);
        printf("Enter resistance R (Ohms):   "); scanf("%lf", &R);
        if (R == 0) { printf("Error: Resistance cannot be zero!\n\n"); return; }
        /* comparing doubles directly to 0 is usually okay for typed input, but tolerance check is cleaner */
        I = V / R;
        printf("\nRESULT: Current I = %.4lf Amperes\n\n", I);
        sprintf(log, "Ohm's Law I=V/R: V=%.2lf V, R=%.2lf Ohm  =>  I=%.4lf A", V, R, I);

    } else if (choice == 3) {
        printf("Enter voltage V (Volts):     "); scanf("%lf", &V);
        printf("Enter current I (Amperes):   "); scanf("%lf", &I);
        if (I == 0) { printf("Error: Current cannot be zero!\n\n"); return; }
        R = V / I;
        printf("\nRESULT: Resistance R = %.4lf Ohms\n\n", R);
        sprintf(log, "Ohm's Law R=V/I: V=%.2lf V, I=%.2lf A  =>  R=%.4lf Ohm", V, I, R);

    } else {
        printf("Invalid choice.\n\n");
        return;
    }
    add_to_history(log);
}

void formula_quadratic() {
    double a, b, c, discriminant, x1, x2;
    char log[300];

    printf("\n--- QUADRATIC EQUATION  (ax^2 + bx + c = 0) ---\n");
    printf("Enter value of a: "); scanf("%lf", &a);
    printf("Enter value of b: "); scanf("%lf", &b);
    printf("Enter value of c: "); scanf("%lf", &c);

    if (a == 0) {
        printf("Error: 'a' cannot be 0 for a quadratic equation.\n\n");
        return;
    }

    discriminant = b*b - 4*a*c;

    printf("\n--- RESULT ---\n");
    if (discriminant > 0) {
        x1 = (-b + sqrt(discriminant)) / (2*a);
        x2 = (-b - sqrt(discriminant)) / (2*a);
        printf("Two real roots:\n");
        printf("  x1 = %.4lf\n", x1);
        printf("  x2 = %.4lf\n\n", x2);
        sprintf(log, "Quadratic %.2lf x^2 + %.2lf x + %.2lf = 0  =>  x1=%.4lf, x2=%.4lf", a, b, c, x1, x2);

    } else if (discriminant == 0) {
        /* float compare with == 0 can be shaky, fabs(discriminant) < small_value is better */
        x1 = -b / (2*a);
        printf("One repeated root:\n");
        printf("  x = %.4lf\n\n", x1);
        sprintf(log, "Quadratic %.2lf x^2 + %.2lf x + %.2lf = 0  =>  x=%.4lf (repeated)", a, b, c, x1);

    } else {
        printf("No real roots (discriminant is negative: %.4lf)\n\n", discriminant);
        /* you could also show complex roots here */
        sprintf(log, "Quadratic %.2lf x^2 + %.2lf x + %.2lf = 0  =>  No real roots", a, b, c);
    }
    add_to_history(log);
}

void formula_circle() {
    double radius, area, circumference;
    char log[300];

    printf("\n--- CIRCLE FORMULAS ---\n");
    printf("Enter radius (any unit): ");
    scanf("%lf", &radius);

    if (radius < 0) {
        printf("Error: Radius cannot be negative.\n\n");
        return;
    }

    area          = PI * radius * radius;
    circumference = 2 * PI * radius;

    printf("\n--- RESULT ---\n");
    printf("  Radius        = %.4lf\n", radius);
    printf("  Area          = PI x r^2      = %.4lf\n", area);
    printf("  Circumference = 2 x PI x r    = %.4lf\n\n", circumference);

    sprintf(log, "Circle r=%.4lf  =>  Area=%.4lf, Circumference=%.4lf", radius, area, circumference);
    add_to_history(log);
}

void formula_pythagoras() {
    int choice;
    double a, b, c;
    char log[300];

    printf("\n--- PYTHAGORAS THEOREM  (a^2 + b^2 = c^2) ---\n");
    printf("1. Find hypotenuse c  (given a and b)\n");
    printf("2. Find side a        (given b and c)\n");
    printf("Choose (1-2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter side a: "); scanf("%lf", &a);
        printf("Enter side b: "); scanf("%lf", &b);
        c = sqrt(a*a + b*b);
        printf("\nRESULT: Hypotenuse c = %.4lf\n\n", c);
        sprintf(log, "Pythagoras: a=%.2lf, b=%.2lf  =>  c=%.4lf", a, b, c);

    } else if (choice == 2) {
        printf("Enter side b: "); scanf("%lf", &b);
        printf("Enter hypotenuse c: "); scanf("%lf", &c);
        if (c < b) { printf("Error: hypotenuse must be larger than side.\n\n"); return; }
        /* also guard c*c - b*b before sqrt, just in case weird input slips through */
        a = sqrt(c*c - b*b);
        printf("\nRESULT: Side a = %.4lf\n\n", a);
        sprintf(log, "Pythagoras: b=%.2lf, c=%.2lf  =>  a=%.4lf", b, c, a);

    } else {
        printf("Invalid choice.\n\n");
        return;
    }
    add_to_history(log);
}

void formula_menu() {
    int choice;

    printf("\n");
    printf("============================================\n");
    printf("            FORMULA SOLVER\n");
    printf("============================================\n");
    printf("  1.  Kinematics      (v, s, u, a, t)\n");
    printf("  2.  Ohm's Law       (V = I x R)\n");
    printf("  3.  Quadratic       (ax^2 + bx + c = 0)\n");
    printf("  4.  Circle          (Area, Circumference)\n");
    printf("  5.  Pythagoras      (a^2 + b^2 = c^2)\n");
    printf("============================================\n");
    printf("Enter choice (1-5): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: formula_kinematics(); break;
        case 2: formula_ohmslaw();    break;
        case 3: formula_quadratic();  break;
        case 4: formula_circle();     break;
        case 5: formula_pythagoras(); break;
        default: printf("Invalid choice.\n\n");
    }
}

/* =========================================
   MAIN MENU
   ========================================= */

int main() {
    int choice;

    printf("\n");
    printf("############################################\n");
    printf("#                                          #\n");
    printf("#   UNIT CONVERTER & FORMULA SOLVER        #\n");
    printf("#   1st Semester C Engineering Project     #\n");
    printf("#                                          #\n");
    printf("############################################\n\n");

    while (1) {
        printf("============================================\n");
        printf("                 MAIN MENU\n");
        printf("============================================\n");
        printf("  1.  Unit Converter\n");
        printf("  2.  Formula Solver\n");
        printf("  3.  View History\n");
        printf("  4.  Save History to File\n");
        printf("  0.  Exit\n");
        printf("============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        /* this is one of the most important fixes, bad input here can mess the full loop */
        printf("\n");

        switch(choice) {
            case 1: unit_converter_menu(); break;
            case 2: formula_menu();        break;
            case 3: show_history();        break;
            case 4: save_history();        break;
            case 0:
                printf("Thank you! Good luck with your submission!\n\n");
                free_history();
                return 0;
            default:
                printf("Invalid choice. Please enter 0 to 4.\n\n");
        }
    }
}