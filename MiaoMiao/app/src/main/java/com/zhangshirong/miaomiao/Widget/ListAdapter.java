package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.databinding.DataBindingUtil;
import android.databinding.ViewDataBinding;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.List;

public class ListAdapter<T,E> extends BaseAdapter{
    private Context context;
    private List<E> list;
    private int layoutId;
    private int variableId;
    private Class<T> vmClass;

    public ListAdapter(Context context,Class<T> vmClass, List<E> list, int layoutId, int variableId) {
        this.context = context;
        this.list = list;
        this.layoutId = layoutId;
        this.variableId = variableId;
        this.vmClass = vmClass;
    }

    @Override
    public int getCount() {
        return list.size();
    }

    @Override
    public Object getItem(int position) {
        return list.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewDataBinding binding = null;
        if (convertView == null){
            binding = DataBindingUtil.inflate(LayoutInflater.from(context),layoutId,parent,false);
        } else {
            binding = DataBindingUtil.getBinding(convertView);
        }
        T vm = null;
        try {
            E itemData = list.get(position);
            vm = vmClass.getConstructor(itemData.getClass()).newInstance(itemData);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        if(vm != null) binding.setVariable(variableId,vm);
        return binding.getRoot();
    }
}
