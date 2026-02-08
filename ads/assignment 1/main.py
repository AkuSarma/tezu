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

def update_marks():
    '''
    Enter student enrolment no and which component to update (e.g., st1, mid) and recomputes total and grade.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()
    print("Enter component to update (st1, mid, st2, end):")
    component = input().strip().lower()

def view_student():
    '''
    Enter student enrolment no and display all details (name, marks, total, grade) in a formatted way.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

def delete_student():
    '''
    Enter student enrolment no. and delete the detail.
    '''
    print("Enter enrolment no.:")
    enrollment_no = input().strip()

def view_all_students():
    '''
    Display a table of all students with enrolment no., Name, ST1, Mid, ST2, End, Total, Grade.
    '''
#     Enrolment No. Name     Test-1 Mid-term Test-2 End-term Total Grade
#     CSD25001      Urmi Das 8      25       9       55       97   O
    print("Enrolment No. | Name       | Test-1 | Mid-term | Test-2 | End-term | Total | Grade")
    print("--------------------------------------------------------------------------")

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