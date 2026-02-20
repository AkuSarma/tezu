import java.util.ArrayList;
import java.util.Scanner;

public class CSE25027_StudentSearch {
    public static void main(String[] args) {
        
        ArrayList<String> students = new ArrayList<>();

        students.add("Aku Sarma");
        students.add("Rahul Das");
        students.add("Tamal Dutta");
        students.add("Gyan mili");
        students.add("Dhiraj kumar");
        students.add("Debasis das");

        Scanner sc = new Scanner(System.in);
        System.out.print("Enter word to search: ");
        String searchWord = sc.nextLine();

        System.out.println("\nStudents whose name contains \"" + searchWord + "\":");

        boolean found = false;

        for (String name : students) {
            if (name.toLowerCase().contains(searchWord.toLowerCase())) {
                System.out.println(name);
                found = true;
            }
        }

        if (!found) {
            System.out.println("No matching student found.");
        }

        sc.close();
    }
}
