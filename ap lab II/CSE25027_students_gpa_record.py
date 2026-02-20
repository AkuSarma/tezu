students = [
    {"name": "Aku Sarma", "GPA": 8.2},
    {"name": "Rahul", "GPA": 6.8},
    {"name": "Gyan", "GPA": 7.4},
    {"name": "Tamal", "GPA": 7.9},
    {"name": "Dhiraj", "GPA": 6.5}
]

found = False

print("\nStudents with GPA less than 7.5:\n")
for student in students:
    if student["GPA"] < 7.5:
        print(f"Name: {student['name']}, GPA: {student['GPA']}")
        found = True

if not found:
    print("No matching student found.")