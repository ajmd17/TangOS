/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tangosimageconverter;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.scene.input.KeyCode;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.DefaultEditorKit;

/**
 *
 * @author User
 */
public class MainForm extends javax.swing.JFrame {

    // The image that will be loaded and clamped
    BufferedImage img = null;
    BufferedImage resizedImg = null;
    String imgName;
    String realImgName;
    int imgWidth = 32, imgHeight = 32;
    ArrayList<Integer> colorsUsed = null; // IDs of the used colors

    public String generateCode() {
        if (img == null) {
            return "";
        }

        realImgName = "";

        boolean invalidCharacters = false;
        for (int i = 0; i < imgName.length(); i++) {
            if (!Character.isAlphabetic(imgName.charAt(i))
                    && !Character.isDigit(imgName.charAt(i))
                    && imgName.charAt(i) != '_') {

                invalidCharacters = true;
            } else {
                realImgName += imgName.charAt(i);
            }
        }
        if (invalidCharacters) {
            JOptionPane.showMessageDialog(this, "\"" + imgName + "\" contains invalid characters.\nThey were removed.");
        }

        if (!Character.isAlphabetic(realImgName.charAt(0)) && realImgName.charAt(0) != '_') {
            JOptionPane.showMessageDialog(this, "Invalid name for variable: \"" + imgName + "\".\nPrefixing variable name with \"img_\".");
            realImgName = "img_" + realImgName;
        }

        txtImgName.setText(realImgName);

        String res = "#ifndef " + realImgName.toUpperCase() + "_H\n";
        res += "#define " + realImgName.toUpperCase() + "_H\n\n";
        res += "static dword " + realImgName + "_width = " + imgWidth + ";\n";
        res += "static dword " + realImgName + "_height = " + imgHeight + ";\n";
        res += "static sbyte " + realImgName + "_data[] = {\n  ";

        int lineLength = 0;
        for (int x = 0; x < imgWidth; x++) {
            for (int y = 0; y < imgHeight; y++) {
                int clr = resizedImg.getRGB(y, x);
                if ((clr >> 24) == 0x00) {
                    res += Integer.toString(-1);
                    lineLength += 1;
                } else {
                    Color color = new Color(clr);
                    Integer index = ImageClamper.USABLE_COLORS.indexOf(color);

                    res += index.toString();
                    lineLength += 1;//index.toString().length();
                }

                if (x < imgWidth - 1 || y < imgHeight - 1) {
                    res += ", ";
                    //lineLength += 2;
                }

                if (lineLength >= imgWidth) {
                    res += "\n  ";
                    lineLength = 0;
                }
            }
        }

        res += "\n};\n";
        res += "\n#endif\n";
        return res;
    }

    private void reloadImage() {
        resizedImg = new BufferedImage(imgWidth, imgHeight, BufferedImage.TYPE_4BYTE_ABGR);
        Graphics2D g = resizedImg.createGraphics();
        g.drawImage(img, 0, 0, imgWidth, imgHeight, null);
        g.dispose();
        g.setComposite(AlphaComposite.Src);
        g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
        g.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);

        int displayWidth = imgWidth * 4;
        int displayHeight = imgHeight * 4;

        lblDisplayingAt.setText("Displaying at: " + displayWidth + "*" + displayHeight);

        imgLabel.setText("");
        imgLabel.setIcon(new ImageIcon(resizedImg.getScaledInstance(displayWidth, displayHeight, Image.SCALE_SMOOTH)));

        txtCode.setText(generateCode());
    }

    private void saveAsSource() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setSelectedFile(new File(realImgName + ".h"));
        fileChooser.setFileFilter(new FileNameExtensionFilter("Header files", "h"));
        fileChooser.setDialogTitle("Save as source");

        int userSelection = fileChooser.showSaveDialog(this);

        if (userSelection == JFileChooser.APPROVE_OPTION) {
            File fileToSave = fileChooser.getSelectedFile();
            String filePath = fileToSave.getAbsolutePath();

            if (filePath.lastIndexOf(".") == -1) {
                filePath += ".h";
            }

            BufferedWriter writer;
            try {
                writer = new BufferedWriter(new FileWriter(filePath));
                writer.write(txtCode.getText());
                writer.close();
            } catch (IOException ex) {
                Logger.getLogger(MainForm.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     * Creates new form MainFrm
     */
    public MainForm() {
        initComponents();
        txtCode.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseReleased(final MouseEvent e) {
                if (e.isPopupTrigger()) {
                    final JPopupMenu menu = new JPopupMenu();

                    JMenuItem copyMenuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
                    copyMenuItem.setText("Copy");
                    copyMenuItem.setEnabled(txtCode.getSelectionStart() != txtCode.getSelectionEnd());
                    menu.add(copyMenuItem);

                    JMenuItem selectAllMenuItem = new JMenuItem();
                    selectAllMenuItem.addActionListener((ActionEvent e1) -> {
                        txtCode.selectAll();
                    });
                    selectAllMenuItem.setText("Select all");
                    selectAllMenuItem.setEnabled(txtCode.getText().length() > 0);
                    menu.add(selectAllMenuItem);

                    menu.add(new JSeparator());

                    JMenuItem saveAsSourceItem = new JMenuItem();
                    saveAsSourceItem.addActionListener((ActionEvent e1) -> {
                        saveAsSource();
                    });
                    saveAsSourceItem.setText("Save as source code");
                    saveAsSourceItem.setEnabled(img != null);
                    menu.add(saveAsSourceItem);

                    menu.show(e.getComponent(), e.getX(), e.getY());
                }
            }
        });
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        txtPath = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jSplitPane1 = new javax.swing.JSplitPane();
        jPanel1 = new javax.swing.JPanel();
        imgLabel = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        txtCode = new javax.swing.JTextPane();
        jComboBox1 = new javax.swing.JComboBox<>();
        jLabel2 = new javax.swing.JLabel();
        lblDisplayingAt = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        txtImgName = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("TangOS Image Converter");

        txtPath.setEditable(false);

        jLabel1.setText("Path:");

        jButton1.setText("...");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jSplitPane1.setDividerLocation(400);
        jSplitPane1.setResizeWeight(0.5);
        jSplitPane1.setCursor(new java.awt.Cursor(java.awt.Cursor.TEXT_CURSOR));

        imgLabel.setBackground(new java.awt.Color(153, 153, 153));
        imgLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        imgLabel.setText("No image loaded");
        imgLabel.setOpaque(true);

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(imgLabel, javax.swing.GroupLayout.DEFAULT_SIZE, 489, Short.MAX_VALUE)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(imgLabel, javax.swing.GroupLayout.DEFAULT_SIZE, 338, Short.MAX_VALUE)
        );

        jSplitPane1.setLeftComponent(jPanel1);

        txtCode.setEditable(false);
        txtCode.setFont(new java.awt.Font("Courier New", 0, 14)); // NOI18N
        jScrollPane1.setViewportView(txtCode);

        jSplitPane1.setRightComponent(jScrollPane1);

        jComboBox1.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Tiny (16*16)", "Small (32*32)", "Medium (64*64)", "Large (128*128)", "Huge (256*256)" }));
        jComboBox1.setSelectedIndex(1);
        jComboBox1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jComboBox1ActionPerformed(evt);
            }
        });

        jLabel2.setLabelFor(jComboBox1);
        jLabel2.setText("Image size:");

        lblDisplayingAt.setText("Displaying at: 128*128");

        jLabel3.setText("Variable name:");

        txtImgName.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusLost(java.awt.event.FocusEvent evt) {
                txtImgNameFocusLost(evt);
            }
        });
        txtImgName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txtImgNameActionPerformed(evt);
            }
        });
        txtImgName.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txtImgNameKeyReleased(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtPath)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(lblDisplayingAt)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 236, Short.MAX_VALUE)
                        .addComponent(jLabel3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtImgName, javax.swing.GroupLayout.PREFERRED_SIZE, 232, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
            .addComponent(jSplitPane1, javax.swing.GroupLayout.Alignment.TRAILING)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(txtPath, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jButton1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel2)
                    .addComponent(lblDisplayingAt)
                    .addComponent(jLabel3)
                    .addComponent(txtImgName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jSplitPane1))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        JFileChooser chooser = new JFileChooser();
        chooser.showOpenDialog(this);

        try {
            File file = chooser.getSelectedFile();
            imgName = file.getName().replaceFirst("[.][^.]+$", "");
            BufferedImage tmpImg = ImageIO.read(file);
            img = tmpImg;
            txtPath.setText(file.getAbsolutePath());
            colorsUsed = ImageClamper.clampImage(img);
            reloadImage();
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Error while loading image: " + e.getMessage());
        }
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jComboBox1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jComboBox1ActionPerformed
        switch (jComboBox1.getSelectedIndex()) {
            case 0:
                imgWidth = 16;
                imgHeight = 16;
                break;
            case 1:
                imgWidth = 32;
                imgHeight = 32;
                break;
            case 2:
                imgWidth = 64;
                imgHeight = 64;
                break;
            case 3:
                imgWidth = 128;
                imgHeight = 128;
                break;
            case 4:
                imgWidth = 256;
                imgHeight = 256;
                break;
        }
        reloadImage();
    }//GEN-LAST:event_jComboBox1ActionPerformed

    private void txtImgNameKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txtImgNameKeyReleased
        if (evt.getKeyCode() == KeyEvent.VK_ENTER) {
            // reload code
            imgName = txtImgName.getText();
            txtCode.setText(generateCode());
        }
    }//GEN-LAST:event_txtImgNameKeyReleased

    private void txtImgNameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txtImgNameActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txtImgNameActionPerformed

    private void txtImgNameFocusLost(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_txtImgNameFocusLost
        if (!imgName.equals(txtImgName.getText())) {
            imgName = txtImgName.getText();
            txtCode.setText(generateCode());
        }
    }//GEN-LAST:event_txtImgNameFocusLost

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.JLabel imgLabel;
    private javax.swing.JButton jButton1;
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JLabel lblDisplayingAt;
    private javax.swing.JTextPane txtCode;
    private javax.swing.JTextField txtImgName;
    private javax.swing.JTextField txtPath;
    // End of variables declaration//GEN-END:variables
}
