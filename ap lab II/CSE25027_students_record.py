# Create empty list to store student records
students = []

# Number of students
n = int(input("Enter number of students: "))

# Taking input from user
for i in range(n):
    print(f"\nEnter details for student {i+1}")
    name = input("Enter student name: ")
    gpa = float(input("Enter student GPA: "))
    
    # Create dictionary for each student
    student = {
        "name": name,
        "GPA": gpa
    }
    
    # Add dictionary to list
    students.append(student)

# Display students with GPA less than 7.5
print("\nStudents with GPA less than 7.5:\n")

found = False
for student in students:
    if student["GPA"] < 7.5:
        print(f"Name: {student['name']}, GPA: {student['GPA']}")
        found = True

if not found:
    print("No student has GPA less than 7.5.")
