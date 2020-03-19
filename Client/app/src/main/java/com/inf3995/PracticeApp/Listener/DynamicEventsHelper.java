// Inspired from https://www.youtube.com/watch?v=094Sv3YrzSU

package com.inf3995.PracticeApp.Listener;

import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;

public class DynamicEventsHelper extends ItemTouchHelper.Callback {
    private DynamicEventsCallback mCallback;

    public DynamicEventsHelper(DynamicEventsCallback callback) {
        mCallback = callback;
    }

    @Override
    public boolean isLongPressDragEnabled() {
        return true;
    }

    @Override
    public boolean isItemViewSwipeEnabled() {
        return true;
    }

    @Override
    public int getMovementFlags(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder) {
        int dragFlags = ItemTouchHelper.UP | ItemTouchHelper.DOWN;
        int swipeFlags = ItemTouchHelper.END | ItemTouchHelper.START;
        return makeMovementFlags(dragFlags, swipeFlags);
    }

    @Override
    public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder, RecyclerView.ViewHolder target) {
        mCallback.onItemMove(viewHolder.getAdapterPosition(), target.getAdapterPosition());
        return true;
    }

    @Override
    public void onSwiped(RecyclerView.ViewHolder viewHolder, int direction) {
        mCallback.removeItem(viewHolder.getAdapterPosition());
    }

    public interface DynamicEventsCallback {
        void onItemMove(int initialPosition, int finalPosition);
        void removeItem(int position);
    }
}
