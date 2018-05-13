package com.zhangshirong.miaomiao.Base.ViewModel;

import android.databinding.BaseObservable;
import android.databinding.BindingAdapter;
import android.widget.GridView;
import android.widget.ListView;

import com.zhangshirong.miaomiao.Widget.ListAdapter;
import com.zhangshirong.miaomiao.Widget.ListViewAutoHeight;

/**
 * Created by kyarvis on 18-4-15.
 */

public class ViewModel extends BaseObservable {
    @BindingAdapter({"adapter"})
    public static void bindList(ListView view, ListAdapter adapter) {
        view.setAdapter(adapter);
    }

    @BindingAdapter({"adapter"})
    public static void bindGrid(GridView view, ListAdapter adapter) {
        view.setAdapter(adapter);
    }
}
