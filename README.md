# Project Site: Visual_without_ros

This is the source for the project site for [Visual_without_ros](https://github.com/geekincode/Visual_without_ros).

> **Note**: This project site is hosted using GitHub Pages, with the `dist/` folder committed to the `gh-pages` branch of this repository. See [the "GitHub Pages" section](#github-pages) below for more information.

## Development

This is a simple Vite + React + TypeScript application. To run it locally:

```bash
npm install
npm run dev
```

Then visit `http://localhost:5173` (or similar).

To build the static assets, run:

```bash
npm run build
```

The resulting build will be in the `dist/ folder.

## Branch Structure

This repository uses multiple branches for different purposes:

- `main`: Contains the ROS project code
- `ph-pages-src`: Contains the React source code for the project homepage (this branch)
- `gh-pages`: Contains the compiled static files for GitHub Pages hosting

## GitHub Pages Deployment

This project uses the [`gh-pages`](https://www.npmjs.com/package/gh-pages) package to deploy the site to GitHub Pages.

The workflow is as follows:

1. Build the site (`npm run build`)
2. Commit the resulting `dist/` folder to the `gh-pages` branch
3. Push the `gh-pages` branch to GitHub

This is all handled by running:

```bash
npm run deploy
```

> **Note**: The `dist/` folder is committed to the `gh-pages` branch, not the `main` branch. This is intentional, as it allows GitHub Pages to serve the built site directly from that branch.

### Git Branch Management

For proper Git management, you should:

1. Keep your ROS source code on the `main` branch
2. Keep your React source code on the `ph-pages-src` branch  
3. Use the `gh-pages` branch only for the compiled static files served by GitHub Pages
4. The `gh-pages` branch is automatically generated and updated by the `npm run deploy` command

> **Important**: Only commit source files to their respective branches. The `gh-pages` branch contains only the compiled output and should not be manually edited.