import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

/**
 * Write a description of class ChangeWorld here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class ChangeWorld extends Button
{
    private final World _new_map;
    public ChangeWorld(GreenfootImage image, World world) {
        super(image);
        _new_map = world;
    }

    /**
     * Act - do whatever the ChangeWorld wants to do. This method is called whenever
     * the 'Act' or 'Run' button gets pressed in the environment.
     */

    @Override
    public void buttonFunc() {
        Greenfoot.setWorld(_new_map);
    }
}
