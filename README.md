Project Description: Hospital Patient Management System with Heap Data Structures

This project is a hospital patient management system designed to prioritize and manage the treatment of patients efficiently. The system reads patient information from a file and processes it using heap data structures for optimal scheduling and priority handling.

Features and Functionality:
Patient Data Management:

Reads patient data, including patient number, name, age, gender, disability rate, priority score, and treatment duration, from an input file.
Stores and sorts patient records based on their registration time for orderly processing.
Heap-Based Priority Queue:

Uses a max-heap to manage and prioritize patient treatment.
Patients are added to the heap based on their calculated priority score.
The heap ensures that the patient with the highest priority is always treated first.
Dynamic Memory Management:

Implements a resizable heap to accommodate dynamic patient loads.
Uses dynamic arrays for storing patient lists, ensuring scalability.
Priority Calculation:

Each patient is assigned a priority score based on specific factors, such as age, disability rate, and medical conditions like bleeding severity.
Older patients, prisoners, and those with severe medical conditions receive higher priority.
Treatment Scheduling:

Patients are treated sequentially based on their priority.
A time-tracking mechanism ensures accurate scheduling and updates the treatment time for each patient.
Visualization and Reporting:

Displays the details of each patient before and after treatment, including their priority score and treatment duration.
Summarizes the completion of all treatments at the end.
Key Algorithms and Data Structures:
Heap Data Structure:
The max-heap is used to ensure that the patient with the highest priority is always at the top, allowing efficient insertion, removal, and priority reorganization.

Bubble Sort:
Used for sorting patient lists based on their registration time for initial organization.

Dynamic Array Resizing:
Ensures efficient memory utilization and accommodates a growing number of patients dynamically.

Advantages of Using Heaps:
Efficiency: Heap-based operations, such as insertion and removal, are performed in logarithmic time, making the system suitable for large datasets.
Optimal Scheduling: The heap structure guarantees that the most critical patients are attended to promptly, improving the quality of healthcare service.
Conclusion:
This project demonstrates the practical application of heap data structures in a healthcare scenario, ensuring efficient and equitable patient treatment. The system is scalable, adaptable, and provides a robust solution for managing hospital workflows.
