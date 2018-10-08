/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package de.dlr.ts.commons.utils.print;

/**
 *
 * @author <a href="mailto:maximiliano.bottazzi@dlr.de">Maximiliano Bottazzi</a>
 */
public class ColorString
{
    private String text;
    private Color fore;
    private Color back;
    private Effect[] effects;
    private static String os = System.getProperty("os.name").toLowerCase().contains("win")?"win":"linux";

    
    /**
     * 
     * @param args 
     */
    public static void main(String[] args)
    {
        System.out.println(Effect.ITALICS.getCode());
    }

    /**
     * 
     */
    public ColorString()
    {
    }

    /**
     * 
     * @param text 
     */
    public ColorString(String text)
    {
        this.text = text;
    }

    /**
     * 
     * @param text 
     */
    public void setText(String text)
    {
        this.text = text;
    }
    
    /**
     * 
     * @param back 
     */
    public void setBackground(Color back)
    {
        this.back = back;
    }
    
    /**
     * 
     * @param text
     * @param fore
     * @param effects 
     */
    public ColorString(String text, Color fore, Effect... effects)
    {
        this.text = text;
        this.fore = fore;
        this.effects = effects;
    }
    
    public ColorString(int text, Color fore, Effect... effects)
    {
        this.text = "" + text;
        this.fore = fore;
        this.effects = effects;
    }
    
    /**
     * 
     * @param text
     * @param fore
     * @param effects
     * @return 
     */
    public static ColorString instance(String text, Color fore, Effect... effects)
    {
        return new ColorString(text, fore, effects);
    }
    
    /**
     * 
     * @param text
     * @param fore
     * @param effects
     * @return 
     */
    public static String string(String text, Color fore, Effect... effects) {
        return new ColorString(text, fore, effects).toString();
    }
    
    public static String string(int text, Color fore, Effect... effects) {
        return new ColorString("" + text, fore, effects).toString();
    }
    
    /**
     * 
     * @param string
     * @param fore
     * @param back
     * @param effects 
     */
    public ColorString(String string, Color fore, Color back, Effect... effects)
    {
        this(string, fore, effects);
        this.back = back;
    }

    /**
     * 
     * @return 
     */
    public String getRawString()
    {
        return text;
    }

    /**
     * 
     * @return 
     */
    public int getRawLength()
    {
        return text.length();
    }
    
    /**
     * 
     * @return 
     */
    public int getOverheadLength()
    {
        return toString().length() - text.length();
    }

    /**
     * 
     * @return 
     */
    @Override
    public String toString()
    {
        return tint(text, fore, effects);
    }

    /**
     * 
     * @param text
     * @param fore
     * @param effects
     * @return 
     */
    private String tint(String text, Color fore, Effect... effects)
    {
        if(os.equals("win")) // || fore.equals(Color.NONE))
            return text;
        
        String tail = "m" + text + "\033[0m";
        String header = "\033[";
        
        if(fore != null && !fore.equals(Color.NONE))
        {
            header += fore.getForegroundCode();
        }
        
        if(effects != null)
        {
            String eff = "";
            for (Effect e : effects)
            {
                eff += ";" + e.getCode();
            }
            header += eff;
        }
        
        if(back != null)
        {
            header += ";" + back.getBackgroundCode();
        }
        
        return header + tail;
    }
    
}
