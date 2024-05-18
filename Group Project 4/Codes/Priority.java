
/**
 * Non-preemptive priority scheduling algorithm.
 *Higher tasks will be executed first
 */

import java.util.*;

// Your code here:
public class Priority implements Algorithm // Implements Algorithm interface, containing schedule() and pickNextTask()
                                           // methods
{
  //Private variables:
  private List<Task> tasklist; // List of tasks
  private Task currentTask;

  //Priority Constructor
  public Priority(List<Task> tasklist) {

    System.out.println("Priority \n");
    this.tasklist = tasklist;

  }

  @Override
  public void schedule(){
    //Sort is important to ensure that the highest priority tasks are executed first
    //Synatx for comparator: Comparator.comparing(keyExtractor)
    tasklist.sort(Comparator.comparing(Task::getPriority));
    while(!tasklist.isEmpty())
      {
        currentTask = pickNextTask();
        CPU.run(currentTask, currentTask.getBurst());
        System.out.println("Task " + currentTask.getName() + " finished\n");
      }
    
  }

  @Override
  public Task pickNextTask(){
    //Removes and returns the first task from the tasklist.(which is the task with the highest priority)
    return tasklist.remove(0);
  }

}