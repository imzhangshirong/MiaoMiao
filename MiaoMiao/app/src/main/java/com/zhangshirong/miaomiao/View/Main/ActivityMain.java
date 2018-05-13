package com.zhangshirong.miaomiao.View.Main;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.databinding.DataBindingUtil;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.FileProvider;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.Base.View.NFragmentActivity;
import com.zhangshirong.miaomiao.Common;
import com.zhangshirong.miaomiao.Config;
import com.zhangshirong.miaomiao.View.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.View.Main.Me.FragmentMeMain;
import com.zhangshirong.miaomiao.View.Main.Note.FragmentNoteMain;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.View.TakeNewNote.ActivityTakeNewNote;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class ActivityMain extends NFragmentActivity implements View.OnClickListener {

    private FragmentNoteMain fragmentNote;
    private FragmentMeMain fragmentMe;

    private RelativeLayout viewNavTakeNewNote;
    private RelativeLayout viewNavNote;
    private RelativeLayout viewNavMe;
    private TextView viewNavNoteIcon;
    private TextView viewNavNoteText;
    private TextView viewNavMeIcon;
    private TextView viewNavMeText;

    private int position =0;
    private String curPhotoPath;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    @Override
    protected void onResume() {
        super.onResume();
        setNav(position);
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityLoading.class);
    }

    private void initView(){
        viewNavTakeNewNote = findViewById(R.id.main_nav_takenewnote);
        viewNavTakeNewNote.setOnClickListener(this);

        viewNavNote = findViewById(R.id.main_nav_note);
        viewNavNoteIcon = findViewById(R.id.main_nav_note_icon);
        viewNavNoteText = findViewById(R.id.main_nav_note_text);
        viewNavNote.setOnClickListener(this);

        viewNavMe = findViewById(R.id.main_nav_me);
        viewNavMeIcon = findViewById(R.id.main_nav_me_icon);
        viewNavMeText = findViewById(R.id.main_nav_me_text);
        viewNavMe.setOnClickListener(this);


        initFragment();
    }

    private void initFragment(){
        FragmentManager fm= getSupportFragmentManager();
        FragmentTransaction transaction =fm.beginTransaction();
        if(fragmentNote!=null){
            transaction.remove(fragmentNote);
        }
        if(fragmentMe!=null){
            transaction.remove(fragmentMe);
        }
        transaction.commit();
        fragmentNote=null;
        fragmentMe=null;
        setNav(position);
    }
    private void setNav(int id){
        position=id;
        FragmentManager fm= getSupportFragmentManager();
        FragmentTransaction transaction =fm.beginTransaction();
        hideFragment(transaction);
        resetNav();
        lightNav(id);
        switch (id){
            case 0:
                if(fragmentNote==null){
                    fragmentNote=new FragmentNoteMain();
                    transaction.add(R.id.main_fragment,fragmentNote);
                }
                transaction.show(fragmentNote);
                break;
            case 1:
                if(fragmentMe==null){
                    fragmentMe=new FragmentMeMain();
                    transaction.add(R.id.main_fragment,fragmentMe);
                }
                transaction.show(fragmentMe);
                break;
            default:
                break;
        }
        transaction.commit();
    }
    private void resetNav(){
        int colorDefault = getResources().getColor(R.color.colorFontNav);
        viewNavNoteIcon.setTextColor(colorDefault);
        viewNavNoteText.setTextColor(colorDefault);
        viewNavMeIcon.setTextColor(colorDefault);
        viewNavMeText.setTextColor(colorDefault);
    }
    private void lightNav(int id){
        int colorLight = getResources().getColor(R.color.colorFontNavLight);
        switch (id){
            case 0:
                viewNavNoteIcon.setTextColor(colorLight);
                viewNavNoteText.setTextColor(colorLight);
                break;
            case 1:
                viewNavMeIcon.setTextColor(colorLight);
                viewNavMeText.setTextColor(colorLight);
                break;
        }
    }

    private void hideFragment(android.support.v4.app.FragmentTransaction transaction){
        if(fragmentNote!=null){
            transaction.hide(fragmentNote);
        }
        if(fragmentMe!=null){
            transaction.hide(fragmentMe);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.main_nav_note:
                if(position!=0)setNav(0);
                break;
            case R.id.main_nav_takenewnote:
                requestCamera();
                break;
            case R.id.main_nav_me:
                if(position!=1)setNav(1);
                break;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode){
            case Common.REQUEST_TAKE_PHOTO:
                if(curPhotoPath!=null){
                    if(resultCode!=0){
                        Bundle bundle = new Bundle();
                        bundle.putString("data",curPhotoPath);
                        startActivity(ActivityTakeNewNote.class,bundle);
                    }
                    else{
                        File file = new File(curPhotoPath);
                        file.delete();
                        curPhotoPath = null;
                    }

                }

                break;
        }
    }

    private void openCamera(){
        Intent intent = new Intent();
        intent.setAction(MediaStore.ACTION_IMAGE_CAPTURE);


        File photoFile = null;
        curPhotoPath = null;
        try {
            photoFile = createImageFile();
        } catch (IOException ex) {
            // Error occurred while creating the File
            Toast.makeText(context,R.string.permission_denied,Toast.LENGTH_LONG).show();
            return;
        }
        // Continue only if the File was successfully created
        if (photoFile != null) {
            curPhotoPath=photoFile.getAbsolutePath();
            Uri photoURI = FileProvider.getUriForFile(context,Config.APP_PACKAGE,photoFile);
            intent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
            startActivityForResult(intent, Common.REQUEST_TAKE_PHOTO);
        }

    }

    private void requestCamera(){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{Manifest.permission.CAMERA,Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0);
            return;
        }
        openCamera();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(requestCode==0){
            boolean allow  = true;
            for(int i=0;i<grantResults.length;i++){
                if(grantResults[i] != PackageManager.PERMISSION_GRANTED){
                    allow = false;
                    break;
                }
            }
            if(allow) {
                openCamera();
            }
            else{
                Toast.makeText(context,R.string.permission_denied,Toast.LENGTH_LONG).show();
            }
        }
    }

    private File createImageFile() throws IOException {
        // Create an image file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = timeStamp;
        File storageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(
                imageFileName,  /* prefix */
                ".jpg",         /* suffix */
                storageDir      /* directory */
        );
        Debug.log(image.getAbsolutePath());
        return image;
    }
}
