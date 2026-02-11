from redis import Redis

r = Redis(host='localhost', port=6379, decode_responses=True)

def compute_grade(total : int) -> str:
    if total >= 95:
        return 'O'
    elif total >= 85:
        return 'A+'
    elif total >= 75:
        return 'A'
    elif total >= 65:
        return 'B+'
    elif total >= 55:
        return 'B'
    elif total >= 45:
        return 'P'
    else:
        return 'F'

def add_student():
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

    if r.exists(enrollment_no):
        print("Student already exists.")
        return

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

    total = st1 + mid + st2 + end

    r.hset(enrollment_no, mapping={
        'name': name,
        'st1': st1,
        'mid': mid,
        'st2': st2,
        'end': end,
        'total': total,
        'grade': compute_grade(total)
    })

    print("Student added successfully.")


def update_marks():
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

    if not r.exists(enrollment_no):
        print("Student not found.")
        return

    print("Enter component to update (st1, mid, st2, end):")
    component = input().strip().lower()

    if component not in ['st1', 'mid', 'st2', 'end']:
        print("Invalid component.")
        return

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

    print("Marks updated successfully.")


def view_student():
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

    data = r.hgetall(enrollment_no)

    if not data:
        print("Student not found.")
        return

    print("Enrolment No. | Name       | Test-1 | Mid-term | Test-2 | End-term | Total | Grade")
    print("-" * 90)

    print(
        f"{enrollment_no:<13} | "
        f"{data.get('name',''):<10} | "
        f"{data.get('st1',''):<6} | "
        f"{data.get('mid',''):<8} | "
        f"{data.get('st2',''):<6} | "
        f"{data.get('end',''):<8} | "
        f"{data.get('total',''):<5} | "
        f"{data.get('grade','')}"
    )


def delete_student():
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

    if r.delete(enrollment_no):
        print("Student deleted successfully.")
    else:
        print("Student not found.")

def view_all_students():
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