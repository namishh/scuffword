#include "utils.h"
#include <stdlib.h>
#include <time.h>

struct Element {
  int at_no;
  char *symbol;
  char *name;
};

struct ElementDay {
  int at_no;
  char *name;
  char *symbol;
  char *date;
};

struct ElementDay ElementOfTheDay;

// rach eleme has its symbol, atomic number and name
const struct Element Elements[] = {
    {1, "H", "Hydrogen"},     {2, "He", "Helium"},     {3, "Li", "Lithium"},
    {4, "Be", "Beryllium"},   {5, "B", "Boron"},       {6, "C", "Carbon"},
    {7, "N", "Nitrogen"},     {8, "O", "Oxygen"},      {9, "F", "Fluorine"},
    {10, "Ne", "Neon"},       {11, "Na", "Sodium"},    {12, "Mg", "Magnesium"},
    {13, "Al", "Aluminium"},  {14, "Si", "Silicon"},   {15, "P", "Phosphorus"},
    {16, "S", "Sulfur"},      {17, "Cl", "Chlorine"},  {18, "Ar", "Argon"},
    {19, "K", "Potassium"},   {20, "Ca", "Calcium"},   {21, "Sc", "Scandium"},
    {22, "Ti", "Titanium"},   {23, "V", "Vanadium"},   {24, "Cr", "Chromium"},
    {25, "Mn", "Manganese"},  {26, "Fe", "Iron"},      {27, "Co", "Cobalt"},
    {28, "Ni", "Nickel"},     {29, "Cu", "Copper"},    {30, "Zn", "Zinc"},
    {31, "Ga", "Gallium"},    {32, "Ge", "Germanium"}, {33, "As", "Arsenic"},
    {34, "Se", "Selenium"},   {35, "Br", "Bromine"},   {36, "Kr", "Krypton"},
    {37, "Rb", "Rubidium"},   {38, "Sr", "Strontium"}, {39, "Y", "Yttrium"},
    {40, "Zr", "Zirconium"},  {41, "Nb", "Niobium"},   {42, "Mo", "Molybdenum"},
    {43, "Tc", "Technetium"}, {44, "Ru", "Ruthenium"}, {45, "Rh", "Rhodium"},
    {46, "Pd", "Palladium"},  {47, "Ag", "Silver"},    {48, "Cd", "Cadmium"},
    {49, "In", "Indium"},     {50, "Sn", "Tin"},       {51, "Sb", "Antimony"}};

void get_element_of_the_day() {
  srand(time(NULL));
  int random = rand() % 50;
  struct Element elem = Elements[random];
  ElementOfTheDay.at_no = elem.at_no;
  ElementOfTheDay.name = elem.name;
  ElementOfTheDay.symbol = elem.symbol;
  int timestamp = (int)time(NULL); // Get current Unix timestamp
  char *time = convert_timestamp(timestamp);
  ElementOfTheDay.date = time;
}

int sumOfAtomicNumbers(char *string) {
  int sum = 0;
  size_t len = strlen(string);

  // check how many NAMES of elemetns are present in the string
  for (int i = 0; i < 50; i++) {
    if (strstr(string, Elements[i].name) != NULL) {
      sum += Elements[i].at_no;
    }
  }

  return sum;
}
