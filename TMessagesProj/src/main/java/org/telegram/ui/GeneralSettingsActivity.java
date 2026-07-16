package org.telegram.ui;

import static org.telegram.messenger.LocaleController.getString;

import android.content.Context;
import android.content.SharedPreferences;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import org.telegram.messenger.MessagesController;
import org.telegram.messenger.R;
import org.telegram.ui.ActionBar.ActionBar;
import org.telegram.ui.ActionBar.BackDrawable;
import org.telegram.ui.ActionBar.BaseFragment;
import org.telegram.ui.ActionBar.Theme;
import org.telegram.ui.Cells.TextCheckCell;
import org.telegram.ui.Components.BulletinFactory;
import org.telegram.ui.Components.LayoutHelper;
import org.telegram.ui.Components.UItem;
import org.telegram.ui.Components.UniversalAdapter;
import org.telegram.ui.Components.UniversalRecyclerView;

import java.util.ArrayList;

public class GeneralSettingsActivity extends BaseFragment {

    private UniversalRecyclerView listView;

    @Override
    public View createView(Context context) {
        actionBar.setBackButtonDrawable(new BackDrawable(false));
        actionBar.setAllowOverlayTitle(true);
        actionBar.setTitle(getString(R.string.SettingsGeneral));
        actionBar.setActionBarMenuOnItemClick(new ActionBar.ActionBarMenuOnItemClick() {
            @Override
            public void onItemClick(int id) {
                if (id == -1) {
                    finishFragment();
                }
            }
        });

        FrameLayout contentView = new FrameLayout(context);
        contentView.setBackgroundColor(Theme.getColor(Theme.key_windowBackgroundGray, resourceProvider));

        listView = new UniversalRecyclerView(this, this::fillItems, this::onClick, this::onLongClick);
        contentView.addView(listView, LayoutHelper.createFrame(LayoutHelper.MATCH_PARENT, LayoutHelper.MATCH_PARENT, Gravity.FILL));

        return fragmentView = contentView;
    }

    private void fillItems(ArrayList<UItem> items, UniversalAdapter adapter) {
        UItem ghostMode = UItem.asCheck(0, getString(R.string.GhostMode)).setChecked(
            MessagesController.getInstance(currentAccount).getMainSettings().getBoolean("ghostMode", false));
        ghostMode.subtext = getString(R.string.GhostModeInfo);
        items.add(ghostMode);
    }

    private void onClick(UItem item, View view, int position, float x, float y) {
        if (item.id == 0) {
            SharedPreferences prefs = MessagesController.getInstance(currentAccount).getMainSettings();
            boolean newValue = !prefs.getBoolean("ghostMode", false);
            prefs.edit().putBoolean("ghostMode", newValue).apply();
            if (view instanceof TextCheckCell) {
                ((TextCheckCell) view).setChecked(newValue);
            }
            listView.adapter.update(false);
            if (newValue) {
                BulletinFactory.of(this).createSimpleBulletin(R.raw.chats_infotip, getString(R.string.GhostMode)).show();
            }
        }
    }

    private boolean onLongClick(UItem item, View view, int position, float x, float y) {
        return false;
    }
}
