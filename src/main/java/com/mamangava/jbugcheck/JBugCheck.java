/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mamangava.jbugcheck;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 *
 * @author etomi
 */
public class JBugCheck {
    
    static {
        try {
            // Roubado da internet ;) copia a lib do jar pra temp, o java não consegue loadar a lib diretamente do .jar
            String mapName = System.mapLibraryName("jbugcheck");
            InputStream is = JBugCheck.class.getResourceAsStream("/native/" + mapName);
            
            if(is == null) {
                System.out.println("Essa porra só vai funcionar no Windows né meooo!");
            } else {
                File libraryFile = File.createTempFile(mapName, ".tmp");
                
                libraryFile.deleteOnExit();
                libraryFile.setWritable(true);
                libraryFile.setExecutable(true);
                
                FileOutputStream os = new FileOutputStream(libraryFile);
                
                int read;
                byte[] buffer = new byte[4096];
                
                while ((read = is.read(buffer)) != -1) {
                    os.write(buffer, 0, read);
                }
                
                os.close();
                is.close();
                
                System.load(libraryFile.getAbsolutePath());
            }
        } catch(IOException e) {
            System.exit(1);
        }
    }

    public static void main(String[] args) {
        new JBugCheck().BugCheck();
    }
    
    private native void BugCheck();
}
