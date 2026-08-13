/*
 * Xournal++
 *
 * Global runtime configuration values shared between the model and control layers.
 *
 * @license GNU GPLv2 or later
 */

#pragma once

namespace xoj::util {

/**
 * Global note-text line spacing factor (Pango semantics). 1.0 = default
 * (normal) spacing; values < 1.0 produce tighter lines, > 1.0 looser.
 *
 * This is a single app-wide setting (persisted in settings.xml), NOT stored
 * per-text-object in the .xopp file. It is read at render time by
 * Text::createPangoLayout(), so it applies uniformly to every text object.
 */
inline double lineSpacing = 0.8;

}  // namespace xoj::util
