#!/bin/bash

echo "Enter City (X): "
read city

echo "Enter Minimum Rating (Y): "
read rating

input_file="text.csv"
output_file="CSE25027_Out6A.txt"

echo "Name | Address | City | Rating | CostForTwo" > "$output_file"

sum=0
count=0

tail -n +2 "$input_file" | while IFS=',' read name address city_name rating_val cost
do
    if [[ -z "$name" ]]; then
        continue
    fi

    if [[ "$city_name" == "$city" ]] && (( $(echo "$rating_val > $rating" | bc -l) ))
    then
        echo "$name | $address | $city_name | $rating_val | $cost" >> "$output_file"
    fi
done

echo "Results written to $output_file"

echo
echo "Part B: Cafe or Hotel from selected list"
echo "----------------------------------------"

grep -E "Cafe|Hotel" "$output_file" | awk -F'|' '{print "Name:",$1,"\nAddress:",$2,"\nRating:",$4,"\n"}'