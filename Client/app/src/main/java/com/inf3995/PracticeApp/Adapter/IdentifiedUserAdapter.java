package com.inf3995.PracticeApp.Adapter;

import android.app.Activity;
import android.support.design.widget.BaseTransientBottomBar;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.SupervisorActivity;
import com.inf3995.PracticeApp.POJO.BlackListUserPOJO;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.ServerAPI;
import com.inf3995.PracticeApp.Service.ServerService;
import com.inf3995.PracticeApp.Service.SupervisorBlackListService;

import java.io.IOException;
import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class IdentifiedUserAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private List<BlackListUserPOJO.User> mList;
    private final Activity mActivity;
    private String mUsername;

    public IdentifiedUserAdapter (List<BlackListUserPOJO.User> userList, Activity activity, String username){
        mList = userList;
        mActivity = activity;
        mUsername = username;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View v = View.inflate(mActivity, R.layout.row_blocked_user_recycler_view, null);
        return new IdentifiedUserAdapter.MyViewHolder(v);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder viewHolder, int i) {
        if (viewHolder instanceof IdentifiedUserAdapter.MyViewHolder) {
            final IdentifiedUserAdapter.MyViewHolder holder = (IdentifiedUserAdapter.MyViewHolder) viewHolder;

            BlackListUserPOJO.User user = mList.get(i);

            holder.userName.setText(user.getNom());
            holder.userIP.setText(user.getIp());
            holder.userMAC.setText(user.getMAC());
            holder.colorBar.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                    int position = viewHolder.getAdapterPosition();
                    Snackbar.make(mActivity.findViewById(R.id.constraint_du), "Please confirm you want to block this user", 2500)
                            .setAction("Block", new View.OnClickListener() {
                                @Override
                                public void onClick(View view) {
                                    int position = viewHolder.getAdapterPosition();
                                    BlackListUserPOJO.User user = mList.get(position);

                                    SupervisorBlackListService blackListService = new SupervisorBlackListService(v.getContext(),
                                            (SupervisorActivity) mActivity, mUsername);
                                    blackListService.addUserToBlock(user);
                                }
                            }).addCallback(new BaseTransientBottomBar.BaseCallback<Snackbar>() {
                        @Override
                        public void onDismissed(Snackbar transientBottomBar, int event) {
                            super.onDismissed(transientBottomBar, event);
                            notifyItemChanged(position);
                        }
                    }).show();
                    return true;
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }

    private class MyViewHolder extends RecyclerView.ViewHolder {
        private final TextView userName, userIP, userMAC, colorBar;

        MyViewHolder(View itemView) {
            super(itemView);
            userName = (TextView) itemView.findViewById(R.id.blocked_user_name);
            userIP = (TextView) itemView.findViewById(R.id.blocked_user_ip);
            userMAC = (TextView) itemView.findViewById(R.id.blocked_user_mac);
            colorBar = (TextView) itemView.findViewById(R.id.blocked_user_color_bar);
        }
    }
}
