package implementations._001.nordshrift.descriptors;

import java.util.ArrayList;

public class NordshriftDescriptorList extends GuardedList
{
    public ArrayList<GuardedList> list = new ArrayList<GuardedList>();

    public void addlist(GuardedList list)
    {
        this.list.add(list);
    }
}
