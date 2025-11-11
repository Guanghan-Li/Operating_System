# Name: Guanghan Li
# Date: 2025-09-23
# Title: Lab1 - Area and Perimeter of Circle
# Description: This program computes the area and the perimeter of the circle with the value of the radius of the circle taken from the user.
#!/bin/sh

circle_area() {
	echo "3.14 * $1 *$1" | bc
}
circle_perimeter() {
	echo "2 * 3.14 * $1" | bc
}

echo "Enter radius of the circle (make sure you input a positive value):"
read radius

if [ $(echo "$radius >0" | bc) -eq 1 ]
then
	echo "Area: $(circle_area $radius)"
	echo "Perimeter: $(circle_perimeter $radius)"
else
	echo "Radius must be positive."
	exit 1
fi
