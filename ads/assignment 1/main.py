from redis import Redis

r = Redis(host='localhost', port=6379, decode_responses=True)

def compute_grade(total : int) -> str:
    if total >= 90:
        return 'O'
    elif total >= 80:
        return 'A'
    elif total >= 70:
        return 'B'
    elif total >= 60:
        return 'C'
    elif total >= 50:
        return 'D'
    else:
        return 'F'

def add_student():
    '''
    Create a new student detail with enrolment no., name, and all four marks. Compute and store total and grade.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()
    print("Enter name:")
    name = input().strip()
    print("Enter marks for:")
    print(" - ST1:")
    st1 = int(input().strip())
    print(" - Mid-term:")
    mid = int(input().strip())
    print(" - ST2:")
    st2 = int(input().strip())
    print(" - End-term:")
    end = int(input().strip())

    r.hset(enrollment_no, mapping={
        'name': name,
        'st1': st1,
        'mid': mid,
        'st2': st2,
        'end': end,
        'total': st1 + mid + st2 + end,
        'grade': compute_grade(st1 + mid + st2 + end)
    })

def update_marks():
    '''
    Enter student enrolment no and which component to update (e.g., st1, mid) and recomputes total and grade.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()
    print("Enter component to update (st1, mid, st2, end):")
    component = input().strip().lower()
    print(f"Enter new marks for {component}:")
    new_marks = int(input().strip())
    r.hset(enrollment_no, component, new_marks)
    
    st1 = int(r.hget(enrollment_no, 'st1') or 0)
    mid = int(r.hget(enrollment_no, 'mid') or 0)
    st2 = int(r.hget(enrollment_no, 'st2') or 0)
    end = int(r.hget(enrollment_no, 'end') or 0)
    total = st1 + mid + st2 + end
    grade = compute_grade(total)
    r.hset(enrollment_no, mapping={'total': total, 'grade': grade})

def view_student():
    '''
    Enter student enrolment no and display all details (name, marks, total, grade) in a formatted way.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()
    student_data = r.hgetall(enrollment_no)
    if not student_data:
        print("Student not found.")
        return
    print("Student Details:")
    for key, value in student_data.items():
        print(f"  {key}: {value}")

def delete_student():
    '''
    Enter student enrolment no. and delete the detail.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()
    r.delete(enrollment_no)

def view_all_students():
    '''
    Display a table of all students with enrolment no., Name, ST1, Mid, ST2, End, Total, Grade.
    '''
    print("Enrolment No. | Name       | Test-1 | Mid-term | Test-2 | End-term | Total | Grade")
    print("-" * 90)

    found = False

    for key in r.scan_iter("*"):
        data = r.hgetall(key)

        # skip non-student / empty hashes (safety)
        if not data or 'name' not in data:
            continue

        found = True

        print(
            f"{key:<13} | "
            f"{data.get('name',''):<10} | "
            f"{data.get('st1',''):<6} | "
            f"{data.get('mid',''):<8} | "
            f"{data.get('st2',''):<6} | "
            f"{data.get('end',''):<8} | "
            f"{data.get('total',''):<5} | "
            f"{data.get('grade','')}"
        )

    if not found:
        print("No student records found.")
("--------------------------------------------------------------------------")

if __name__ == '__main__':
    while(True):
        print("Enter your choice:\n1. Add Student\n2. Update Marks\n3. View Student\n4. Delete Student\n5. View all student\n6. Exit")
        choice = int(input())
        if choice == 1:
            add_student()
        elif choice == 2:
            update_marks()
        elif choice == 3:
            view_student()
        elif choice == 4:
            delete_student()
        elif choice == 5:
            view_all_students()
        else:
            break

    r.close()